ENTRY(CosmOS)
SECTIONS
{
	.text 0xFFFF800000000000 :
	{
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}
	.data :
	{
		_tone_s = .;
		KEEP(*(.rawdata*))
		_tone_e = .;
		. = ALIGN(4096);
	}
	.bss :
	{
		*(.bss)
		*(COMMON)
		_ebss = .;
		. = ALIGN(4096);
	}
	_end = .;
}