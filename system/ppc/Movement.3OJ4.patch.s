.meta name="Movement"
.meta description="Allow backsteps and\nmovement when enemies\nare nearby"
# Original code by Ralf @ GC-Forever

entry_ptr:
reloc0:
  .offsetof start
start:
  .include  WriteCodeBlocks
  # region @ 801D0D10 (4 bytes)
  .data     0x801D0D10  # address
  .data     0x00000004  # size
  .data     0x4800000C  # 801D0D10 => b         +0x0000000C /* 801D0D1C */
  # region @ 801D1CEC (4 bytes)
  .data     0x801D1CEC  # address
  .data     0x00000004  # size
  .data     0x48000014  # 801D1CEC => b         +0x00000014 /* 801D1D00 */
  # end sentinel
  .data     0x00000000  # address
  .data     0x00000000  # size
