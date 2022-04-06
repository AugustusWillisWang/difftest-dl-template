#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define check_and_assert(func)                                \
  do {                                                        \
    if (!func) {                                              \
      printf("ERROR: %s\n", dlerror());  \
      assert(func);                                           \
    }                                                         \
  } while (0);

using namespace std;

int main(int argc, const char** argv)
{
  void *handle = dlmopen(LM_ID_NEWLM, "../NutShell/build/emu.so", RTLD_LAZY | RTLD_DEEPBIND);
  if(!handle){
    printf("%s\n", dlerror());
    assert(0);
  }

  auto core_emu_init = (int (*)(int, const char**))dlsym(handle, "core_emu_init");
  check_and_assert(core_emu_init);
  auto core_emu_finished = (bool (*)())dlsym(handle, "core_emu_finished");
  check_and_assert(core_emu_finished);
  auto core_emu_single_step = (int (*)())dlsym(handle, "core_emu_single_step");
  check_and_assert(core_emu_single_step);
  auto core_emu_n_step = (int (*)())dlsym(handle, "core_emu_n_step");
  check_and_assert(core_emu_n_step);
  auto core_emu_cleanup = (void (*)())dlsym(handle, "core_emu_cleanup");
  check_and_assert(core_emu_cleanup);
  auto core_pmem_host_addr_map = (uint64_t *(*)(uint64_t))dlsym(handle, "pmem_host_addr_map");
  check_and_assert(core_pmem_host_addr_map);

  // // Example 1
  // // Run emulator in dl
  // auto run_emu = (int (*)(int, const char**))dlsym(handle, "run_emu");
  // check_and_assert(run_emu);
  // run_emu(argc, argv);

  // // Example 2
  // // Run emulator in dl cycle by cycle 
  // // Init emulator
  // core_emu_init(argc, argv);
  // // Run emulator
  // while (!core_emu_finished()) {
  //   if(core_emu_single_step() != 0)
  //     break; // break if execute_single_cycle requires abortion
  // }
  // // Simulation ends here, do clean up & display jobs
  // core_emu_cleanup();

  // Example 3
  // Run emulator in dl cycle by cycle, and read/write dut processor pmem 
  // Init emulator
  core_emu_init(argc, argv);
  // Run emulator
  uint64_t* host_addr = core_pmem_host_addr_map(0x80000000); // get core pmem addr in host machine 
  printf("data at %p is %lx\n", host_addr, *host_addr); // pmem read
  *host_addr = 0x0000006b; // pmem write
  // Note that core has caches, core may not notice modification in pmem immediately
  while (!core_emu_finished()) {
    if(core_emu_single_step() != 0)
      break; // break if execute_single_cycle requires abortion
  }
  // Simulation ends here, do clean up & display jobs
  core_emu_cleanup();

  // Example 4
  // TODO: manually control wave dump

  // Example 5
  // TODO: run isa simulator only
}