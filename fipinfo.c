#include <assert.h>
#include <lzma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fip_header {
  char magic1[8];                 // 0
  uint32_t magic2;                // 8
  uint32_t param_cksum;           // c
  uint8_t nand_info[128];         // 10
  uint8_t nor_info[36];           // 90
  uint8_t fip_flags[8];           // b4
  uint32_t chip_conf_size;        // bc

  uint32_t blcp_img_cksum;        // c0
  uint32_t blcp_img_size;         // c4
  uint32_t blcp_img_runaddr;      // c8

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
  uint64_t runaddr;
  uint32_t reserved1;
  uint32_t reserved2;
};

void unpack_uboot(struct ld2_2nd_header *h3, uint8_t *compressed, uint32_t compressed_size) {
  lzma_ret lzret;
  lzma_stream lzs = LZMA_STREAM_INIT;

  lzret = lzma_alone_decoder(&lzs, UINT64_MAX);
  assert(lzret == LZMA_OK);

  uint32_t out_size = 1024*1024;
  uint8_t *output = malloc(out_size);

  lzs.next_in = compressed;
  lzs.avail_in = compressed_size;
  lzs.next_out = output;
  lzs.avail_out = out_size;

  lzret = lzma_code(&lzs, LZMA_RUN);
  printf("total out: %ld, ret=%d\n", lzs.total_out, lzret);
  assert(lzret == LZMA_STREAM_END);

  FILE *uboot = fopen("uboot-out.bin", "w");
  memcpy(&h3->magic, "BL33", 4);
  fwrite(h3, 0x20, 1, uboot);
  fwrite(output, lzs.total_out, 1, uboot);
  fclose(uboot);
}

void unpack_bare(FILE *fd, uint32_t off, uint32_t size, char *name) {
  uint8_t *buf = malloc(size);
  fseek(fd, off, SEEK_SET);
  fread(buf, size, 1, fd);

  FILE *out = fopen(name, "w");
  fwrite(buf, size, 1, out);
  fclose(out);

  free(buf);
}

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

  printf("header1 size: 0x%x\n", (uint32_t)sizeof(header));

  fseek(fd, header.param2_loadaddr, SEEK_SET);
  fread(&header2, sizeof(header2), 1, fd);

  printf("ddr param cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, reserved: 0x%x\n", header2.ddr_param_cksum, header2.ddr_param_loadaddr, header2.ddr_param_size, header2.ddr_param_reserved);
  printf("blcp 2nd cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, runaddr: 0x%x\n", header2.blcp_2nd_cksum, header2.blcp_2nd_loadaddr, header2.blcp_2nd_size, header2.blcp_2nd_runaddr);
  printf("monitor cksum: 0x%x, loadaddr: 0x%x, size: 0x%x, runaddr: 0x%x\n", header2.monitor_cksum, header2.monitor_loadaddr, header2.monitor_size, header2.monitor_runaddr);
  printf("loader 2nd reserved0: 0x%x, loadaddr: 0x%x, reserved1: 0x%x, reserved2: 0x%x\n", header2.loader_2nd_reserved0, header2.loader_2nd_loadaddr, header2.loader_2nd_reserved1, header2.loader_2nd_reserved2);

  struct ld2_2nd_header h3;
  fseek(fd, header2.loader_2nd_loadaddr, SEEK_SET);
  fread(&h3, sizeof(h3), 1, fd);
  char h3_magic[5];
  memcpy(h3_magic, &h3.magic, 4);
  h3_magic[4] = 0;
  printf("loader 2nd jump0: 0x%x, magic: %s cksum: 0x%x, size: %d, runaddr: 0x%lx, reserved1: 0x%x, reserved2: 0x%x\n", h3.jump0, h3_magic, h3.cksum, h3.size, h3.runaddr, h3.reserved1, h3.reserved2);

  uint8_t *uboot_compressed = malloc(h3.size);
  fseek(fd, header2.loader_2nd_loadaddr + 0x20, SEEK_SET);
  fread(uboot_compressed, h3.size, 1, fd);

  unpack_uboot(&h3, uboot_compressed, h3.size);

  // just duo-buildroot-sdk/fsbl/test/cv181x/ddr_param.bin un-modified
  //unpack_bare(fd, header2.ddr_param_loadaddr, header2.ddr_param_size, "ddr-out.bin");

  // just opensbi fw_dynamic.bin, tail padded to 1kb
  //unpack_bare(fd, header2.monitor_loadaddr, header2.monitor_size, "monitor-out.bin");

  if (header2.blcp_2nd_loadaddr) {
    unpack_bare(fd, header2.blcp_2nd_loadaddr, header2.blcp_2nd_size, "blcp_2nd-out.bin");
  }

  // contains files such as fsbl/plat/cv180x/usb/dwc2_udc_otg.c
  unpack_bare(fd, sizeof(header) + header.blcp_img_size, header.bl2_img_size, "bl2-out.bin");
  return 0;
}
