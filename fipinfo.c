#include <stdio.h>
#include <stdint.h>

struct fip_header {
  char magic1[8];
  uint32_t magic2;
  uint32_t param_cksum;
  uint8_t nand_info[128];
  uint8_t nor_info[36];
  uint8_t fip_flags[8];
  uint32_t chip_conf_size;
  uint32_t blcp_img_cksum;
  uint32_t blcp_img_size;
  uint32_t blcp_img_runaddr;

  uint32_t blcp_param_loadaddr;
  uint32_t blcp_param_size;

  uint32_t bl2_img_cksum;
  uint32_t bl2_img_size;
  uint32_t bld_img_size;
  uint32_t param2_loadaddr;

  uint32_t reserved1;

  uint8_t chip_conf[760];
  uint8_t bl_ek[32];
  uint8_t root_pk[512];
  uint8_t bl_pk[512];
  uint8_t bl_pk_sig[512];
  uint8_t chip_conf_sig[512];
  uint8_t bl2_img_sig[512];
  uint8_t blcp_img_sig[512];
};

struct fip_header2 {
  char magic1[8];
  uint32_t param2_cksum;
  uint8_t reserved1[4];

  uint32_t ddr_param_cksum;
  uint32_t ddr_param_loadaddr;
  uint32_t ddr_param_size;
  uint32_t ddr_param_reserved;

  uint32_t blcp_2nd_cksum;
  uint32_t blcp_2nd_loadaddr;
  uint32_t blcp_2nd_size;
  uint32_t blcp_2nd_runaddr;

  uint32_t monitor_cksum;
  uint32_t monitor_loadaddr;
  uint32_t monitor_size;
  uint32_t monitor_runaddr;

  uint32_t loader_2nd_reserved0;
  uint32_t loader_2nd_loadaddr;
  uint32_t loader_2nd_reserved1;
  uint32_t loader_2nd_reserved2;
};

struct ld2_2nd_header {
  uint32_t jump0;
  uint32_t magic;
  uint32_t cksum;
  uint32_t size;
  uint32_t runaddr;
  uint32_t reserved1;
  uint32_t reserved2;
};

int main(int argc, char **argv) {
  FILE *fd = fopen(argv[1], "r");
  struct fip_header header;
  struct fip_header2 header2;
  fread(&header, sizeof(header), 1, fd);
  printf("magic1: %s", header.magic1);
  printf("magic2: 0x%x\n", header.magic2);
  printf("param_cksum: 0x%x\n", header.param_cksum);

  printf("chip conf size: %d\n", header.chip_conf_size);
  printf("blcp img chsum: 0x%x, size: %d, runaddr: 0x%x\n", header.blcp_img_cksum, header.blcp_img_size, header.blcp_img_runaddr);
  printf("blcp param size: %d, loadaddr: 0x%x\n", header.blcp_param_size, header.blcp_param_loadaddr);
  printf("bl2 cksum: 0x%x, size: %d\n", header.bl2_img_cksum, header.bl2_img_size);
  printf("param2 loadaddr: 0x%x\n", header.param2_loadaddr);

  printf("header1 size: 0x%x\n", sizeof(header));

  fseek(fd, header.param2_loadaddr, SEEK_SET);
  fread(&header2, sizeof(header2), 1, fd);

  printf("ddr param cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, reserved: 0x%x\n", header2.ddr_param_cksum, header2.ddr_param_loadaddr, header2.ddr_param_size, header2.ddr_param_reserved);
  printf("blcp 2nd cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, runaddr: 0x%x\n", header2.blcp_2nd_cksum, header2.blcp_2nd_loadaddr, header2.blcp_2nd_size, header2.blcp_2nd_runaddr);
  printf("monitor cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, runaddr: 0x%x\n", header2.monitor_cksum, header2.monitor_loadaddr, header2.monitor_size, header2.monitor_runaddr);
  printf("loader 2nd reserved0: 0x%x, loadaddr: 0x%x, reserved1: 0x%x, reserved2: 0x%x\n", header2.loader_2nd_reserved0, header2.loader_2nd_loadaddr, header2.loader_2nd_reserved1, header2.loader_2nd_reserved2);
  return 0;
}
