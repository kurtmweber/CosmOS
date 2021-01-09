/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/ata/ata_identity.h>
#include <dev/i386/ata/ata_util.h>
#include <sys/asm/asm.h>
#include <sys/i386/mm/mm.h>

uint32_t ata_detect_extract_dword(const char *identify_buf, ata_identify_offsets offset){
	return *((uint32_t *)(&identify_buf[offset]));
}

uint64_t ata_detect_extract_qword(const char *identify_buf, ata_identify_offsets offset){
	return *((uint64_t *)(&identify_buf[offset]));
}

uint16_t ata_detect_extract_word(const  char *identify_buf, ata_identify_offsets offset){
	return *((uint16_t *)(&identify_buf[offset]));
}

char *ata_detect_extract_string(const char *identify_buf, uint8_t len, ata_identify_offsets offset){
	char *c;
	uint8_t i;
	
	if (len % 2){
		return (char *)0;
	}
	
	c = (char *)kmalloc((len + 1) * sizeof(char));
	
	for (i = 0; i < (len / 2); i++){
		// for whatever reason, ATA IDENTIFY character string data is big-endian words, so we've got to flip every set of two bytes
		c[(2 * i)] = identify_buf[(2 * i) + offset + 1];
		c[(2 * i) + 1] = identify_buf[(2 * i) + offset];
	}
	
	c[len] = '\0';
	
	return c;
}

uint32_t ata_detect_sector_size(const char *identify_buf){
	uint16_t plss_word, version_word;
	
	version_word = ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_MAJOR_VERSION);
	if (!(version_word & (1 << 7))){		// if major version is not 7 or greater (signified by bit 7 being set), then the device definitely does not support large sectors
		return 512;
	}
	
	plss_word = ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_PLSS);
	
	if ((plss_word & 0x4000) != 0x4000){	// if bit 15 = 0 and bit 14 = 1, then the word contains valid data; if it doesn't, then we know the device does not support large sectors, which is an optional feature in versions 7+
		return 512;
	}
	
	if (!(plss_word & (1 << 12))){		// bit 12 being set signifies support for logical sector greater than 256 words (512 bytes), so if it's not set then we know it's 512
		return 512;
	}
	
	// If we reach this point then we know that the device supports logical sectors > 512 bytes, so we check to see what it is
	
	return ata_detect_extract_dword(identify_buf, ATA_IDENTIFY_OFFSET_SECTOR_SIZE);
}

char *ata_detect_read_identify(struct ata_controller* controller, uint8_t channel){
	uint16_t *buf;
	uint16_t i;
	buf = (uint16_t *)kmalloc(512);	// we request 512 bytes, but keep in mind that it's a uint16 so it's a 256-element array
	
	for (i = 0; i < 256; i++){
		buf[i] = ata_register_read_word(controller, channel, ATA_REGISTER_DATA);
	}
	return (char *)buf;
}

void ata_extract_identity(const char* identity, struct ata_device* dev) {
	if (ata_detect_extract_word(identity, ATA_IDENTIFY_OFFSET_COMMAND_SET_2) & (1 << 10)){
		dev->size = ata_detect_extract_dword(identity, ATA_IDENTIFY_OFFSET_LBA) * 512;
	} else {
		dev->size = ata_detect_extract_qword(identity, ATA_IDENTIFY_OFFSET_LBA_EXT) * 512;
	}
	dev->removable = (ata_detect_extract_word(identity , ATA_IDENTIFY_OFFSET_GENERAL) & (1 << 7)) >> 7;
	dev->bytes_per_sector = ata_detect_sector_size(identity );
	dev->model=ata_detect_extract_string(identity , 40, ATA_IDENTIFY_OFFSET_MODEL);
	dev->serial=ata_detect_extract_string(identity , 20, ATA_IDENTIFY_OFFSET_SERIAL);
}

