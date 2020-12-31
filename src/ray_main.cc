#include <omp.h>  // for openMP

#include "raycer.hh"

struct ray_trace_opt_t {
  int width, height, sppm;
  const char* filename;

  void detail() {
    printf("tracing options: %d x %d, %d per pixel, to '%s'\n", width, height,
           sppm, filename);
  }
};

ray_trace_opt_t parseCommandLine(int argc, char** argv) {
  if (argc != 5) {
    fprintf(stderr, "Failed to parse command line arguments\n");
    fprintf(stderr, "use [width] [height] [sample_per_pixel] [output_name]\n");
    exit(EXIT_FAILURE);
  }
  ray_trace_opt_t ret;
  ret.width = atoi(argv[1]);
  ret.height = atoi(argv[2]);
  ret.sppm = atoi(argv[3]);
  ret.filename = argv[4];
  return ret;
}

int main(int argc, char** argv) {
  auto timer = raycer::Timer();

  auto opt = parseCommandLine(argc, argv);
  opt.detail();

  timer.start();
  printf("tracing starts\n");

  auto image = raycer::BMPImage(opt.width, opt.height, true);

  auto timeUsed = timer.stop();
  printf("tracing finished with %g\n", timeUsed);
  return 0;
}