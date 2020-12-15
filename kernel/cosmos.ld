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