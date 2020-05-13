ENTRY(JustOS)
OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(binary)
OUTPUT(justos)

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