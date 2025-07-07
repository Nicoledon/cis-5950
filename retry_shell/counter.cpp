#include <cstdlib>  // for abort , EXIT_SUCCESS, EXIT_FAILURE and exit;
#include <random>   // for random_device, mt19937_64, uniform_int_distribution
#include <iostream> // for cout, cerr, and endl
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <signal.h>
#include <sys/types.h>
#include <cstdint>  // for uint8_t

using namespace std;

constexpr uint8_t limit = 5;
constexpr const char* path = "/tmp/.5950-counter";
constexpr int mode = 0644;

// this program uses a temporary file to keep trach of how often this
// program was run.
int main(int argc, char* argv[]) {
  int fd = open(path, O_RDWR | O_CREAT, mode);

  uint8_t counter = 0;
  ssize_t res = read(fd, &counter, sizeof(uint8_t));

  counter += 1;
  if (res == 0 || counter < limit) {
    lseek(fd, 0, SEEK_SET);
    res = write(fd, &counter, sizeof(uint8_t));
    close(fd);

    cerr << "Not done yet..." << endl;

    // send SIGKILL to self
    kill(getpid(), SIGKILL);
  } else {
    close(fd);
    int res = unlink(path);
    if (res != 0) {
      cerr << strerror(errno) << endl;
      cerr << "error unlink failed, get Travis" << endl;
    }
  }

  return EXIT_SUCCESS;
}
