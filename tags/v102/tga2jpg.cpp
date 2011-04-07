// tga2jpg.cpp
// Public domain, Rich Geldreich <richgel99@gmail.com>
// Last updated Dec. 18, 2010

#include "jpge.h"
#include "stb_image.c"

static uint get_file_size(const char *pFilename)
{
   FILE *pFile = fopen(pFilename, "rb");
   if (!pFile) return 0;
   fseek(pFile, 0, SEEK_END);
   uint file_size = ftell(pFile);
   fclose(pFile);
   return file_size;
}

int main(int arg_c, char* Parg_v[])
{
   printf("jpeg_encoder class example program\n");

   if (arg_c != 4)
   {
      printf("Usage: jpge <sourcefile> <destfile> <quality_factor>\n");
      printf("sourcefile: Source image file, in any format stb_image.c supports.\n");
      printf("destfile: Destination JPEG file.\n");
      printf("quality_factor: 1-100, higher=better\n");
      return EXIT_FAILURE;
   }

   const char* pSrc_filename = Parg_v[1];
   const char* pDst_filename = Parg_v[2];
   int quality_factor = atoi(Parg_v[3]);
   if ((quality_factor < 1) || (quality_factor > 100))
   {
      printf("Quality factor must range from 1-100!\n");
      return EXIT_FAILURE;
   }
   
   // Which helper method to test.
   const bool test_memory_compression = false;
   
   // Load the source image.
   const int req_comps = 3;
   int width = 0, height = 0, actual_comps = 0;
   uint8 *pImage_data = stbi_load(pSrc_filename, &width, &height, &actual_comps, req_comps);
   if (!pImage_data)
   {
      printf("Failed loading file \"%s\"!\n", pSrc_filename);
      return EXIT_FAILURE;
   }

   printf("Source image resolution: %ix%i\n", width, height);

   // Fill in the compression parameter structure.
   jpge::params params;
   params.m_quality = quality_factor; 
   params.m_subsampling = (actual_comps == 1) ? jpge::Y_ONLY : jpge::H2V2;

   printf("Writing JPEG image to file: %s\n", pDst_filename);

   // Now create the JPEG file.

   if (test_memory_compression)
   {
      int buf_size = width * height * 3; // allocate a buffer that's hopefully big enough (this is way overkill for jpeg)
      if (buf_size < 1024) buf_size = 1024;
      void *pBuf = malloc(buf_size); 
      
      if (!compress_image_to_jpeg_file_in_memory(pBuf, buf_size, width, height, req_comps, pImage_data, params))
      {
         printf("Failed creating JPEG data!\n");
         return EXIT_FAILURE;
      }

      FILE *pFile = fopen(pDst_filename, "wb");
      if (!pFile)
      {
         printf("Failed creating file \"%s\"!\n", pDst_filename);
         return EXIT_FAILURE;
      }
   
      if (fwrite(pBuf, buf_size, 1, pFile) != 1)
      {
         printf("Failed writing to output file!\n");
         return EXIT_FAILURE;
      }

      if (fclose(pFile) == EOF)
      {
         printf("Failed writing to output file!\n");
         return EXIT_FAILURE;
      }
   }
   else
   {
      if (!compress_image_to_jpeg_file(pDst_filename, width, height, req_comps, pImage_data, params))
      {
         printf("Failed writing to output file!\n");
         return EXIT_FAILURE;
      }
   }

   printf("Compressed file size: %u\n", get_file_size(pDst_filename));

   // Now try loading the JPEG file using stbi_image's JPEG decompressor.
  
   int uncomp_width = 0, uncomp_height = 0, uncomp_actual_comps = 0, uncomp_req_comps = 3;
   uint8 *pUncomp_image_data = stbi_load(pDst_filename, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
   if (!pUncomp_image_data)
   {
      printf("Failed loading compressed image file \"%s\"!\n", pDst_filename);
      return EXIT_FAILURE;
   }

   if ((uncomp_width != width) || (uncomp_height != height))
   {
      printf("Loaded JPEG file has a different resolution than the original file!\n");
      return EXIT_FAILURE;
   }

   if ((req_comps == 3) && (uncomp_req_comps == 3))
   {
      // Compute image error metrics.
      double hist[256];
      memset(hist, 0, sizeof(hist));
      
      const uint num_channels = 3;
      const uint first_channel = 0;

      for (int y = 0; y < height; y++)
      {
         for (int x = 0; x < width; x++)
         {
            const uint8* pA = pImage_data + y * width * req_comps + x * req_comps;
            const uint8* pB = pUncomp_image_data + y * uncomp_width * uncomp_req_comps + x * uncomp_req_comps;

            for (uint c = 0; c < num_channels; c++)
               hist[labs(pA[first_channel + c] - pB[first_channel + c])]++;
         }
      }

      double max_err = 0;
      double sum = 0.0f, sum2 = 0.0f;
      for (uint i = 0; i < 256; i++)
      {
         if (!hist[i])
            continue;

         if (i > max_err) max_err = i;

         double x = i * hist[i];

         sum += x;
         sum2 += i * x; 
      }

      // See http://bmrc.berkeley.edu/courseware/cs294/fall97/assignment/psnr.html
      double total_values = width * height;

      const bool average_component_error = false;
      if (average_component_error)
         total_values *= num_channels;

      double mean = sum / total_values;
      double mean_squared = sum2 / total_values;

      double root_mean_squared = sqrt(mean_squared);

      double peak_snr;
      if (!root_mean_squared)
         peak_snr = 1e+10f;
      else
         peak_snr = log10(255.0f / root_mean_squared) * 20.0f;
      
      printf("Error Max: %f, Mean: %f, Mean^2: %f, RMSE: %f, PSNR: %f\n", max_err, mean, mean_squared, root_mean_squared, peak_snr);
   }

   printf("Success.\n");

   return EXIT_SUCCESS;
}



