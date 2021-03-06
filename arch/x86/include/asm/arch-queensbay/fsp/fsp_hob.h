/*
 * Copyright (C) 2013, Intel Corporation
 * Copyright (C) 2014, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	Intel
 */

#ifndef __FSP_HOB_H__
#define __FSP_HOB_H__

/* Type of HOB Header */
#define HOB_TYPE_MEM_ALLOC	0x0002
#define HOB_TYPE_RES_DESC	0x0003
#define HOB_TYPE_GUID_EXT	0x0004
#define HOB_TYPE_UNUSED		0xFFFE
#define HOB_TYPE_EOH		0xFFFF

/*
 * Describes the format and size of the data inside the HOB.
 * All HOBs must contain this generic HOB header.
 */
struct hob_header_t {
	u16	type;		/* HOB type */
	u16	len;		/* HOB length */
	u32	reserved;	/* always zero */
};

/* Enumeration of memory types introduced in UEFI */
enum efi_mem_type_t {
	EFI_RESERVED_MEMORY_TYPE,
	/*
	 * The code portions of a loaded application.
	 * (Note that UEFI OS loaders are UEFI applications.)
	 */
	EFI_LOADER_CODE,
	/*
	 * The data portions of a loaded application and
	 * the default data allocation type used by an application
	 * to allocate pool memory.
	 */
	EFI_LOADER_DATA,
	/* The code portions of a loaded Boot Services Driver */
	EFI_BOOT_SERVICES_CODE,
	/*
	 * The data portions of a loaded Boot Serves Driver and
	 * the default data allocation type used by a Boot Services
	 * Driver to allocate pool memory.
	 */
	EFI_BOOT_SERVICES_DATA,
	/* The code portions of a loaded Runtime Services Driver */
	EFI_RUNTIME_SERVICES_CODE,
	/*
	 * The data portions of a loaded Runtime Services Driver and
	 * the default data allocation type used by a Runtime Services
	 * Driver to allocate pool memory.
	 */
	EFI_RUNTIME_SERVICES_DATA,
	/* Free (unallocated) memory */
	EFI_CONVENTIONAL_MEMORY,
	/* Memory in which errors have been detected */
	EFI_UNUSABLE_MEMORY,
	/* Memory that holds the ACPI tables */
	EFI_ACPI_RECLAIM_MEMORY,
	/* Address space reserved for use by the firmware */
	EFI_ACPI_MEMORY_NVS,
	/*
	 * Used by system firmware to request that a memory-mapped IO region
	 * be mapped by the OS to a virtual address so it can be accessed by
	 * EFI runtime services.
	 */
	EFI_MMAP_IO,
	/*
	 * System memory-mapped IO region that is used to translate
	 * memory cycles to IO cycles by the processor.
	 */
	EFI_MMAP_IO_PORT,
	/*
	 * Address space reserved by the firmware for code that is
	 * part of the processor.
	 */
	EFI_PAL_CODE,
	EFI_MAX_MEMORY_TYPE
};

/*
 * Describes all memory ranges used during the HOB producer phase that
 * exist outside the HOB list. This HOB type describes how memory is used,
 * not the physical attributes of memory.
 */
struct hob_mem_alloc_t {
	struct hob_header_t	hdr;
	/*
	 * A GUID that defines the memory allocation region's type and purpose,
	 * as well as other fields within the memory allocation HOB. This GUID
	 * is used to define the additional data within the HOB that may be
	 * present for the memory allocation HOB. Type efi_guid_t is defined in
	 * InstallProtocolInterface() in the UEFI 2.0 specification.
	 */
	struct efi_guid_t	name;
	/*
	 * The base address of memory allocated by this HOB.
	 * Type phys_addr_t is defined in AllocatePages() in the UEFI 2.0
	 * specification.
	 */
	phys_addr_t		mem_base;
	/* The length in bytes of memory allocated by this HOB */
	phys_size_t		mem_len;
	/*
	 * Defines the type of memory allocated by this HOB.
	 * The memory type definition follows the EFI_MEMORY_TYPE definition.
	 * Type EFI_MEMORY_TYPE is defined in AllocatePages() in the UEFI 2.0
	 * specification.
	 */
	enum efi_mem_type_t	mem_type;
	/* padding */
	u8			reserved[4];
};

/* Value of ResourceType in HOB_RES_DESC */
#define RES_SYS_MEM		0x00000000
#define RES_MMAP_IO		0x00000001
#define RES_IO			0x00000002
#define RES_FW_DEVICE		0x00000003
#define RES_MMAP_IO_PORT	0x00000004
#define RES_MEM_RESERVED	0x00000005
#define RES_IO_RESERVED		0x00000006
#define RES_MAX_MEM_TYPE	0x00000007

/*
 * These types can be ORed together as needed.
 *
 * The first three enumerations describe settings
 * The rest of the settings describe capabilities
 */
#define RES_ATTR_PRESENT			0x00000001
#define RES_ATTR_INITIALIZED			0x00000002
#define RES_ATTR_TESTED				0x00000004
#define RES_ATTR_SINGLE_BIT_ECC			0x00000008
#define RES_ATTR_MULTIPLE_BIT_ECC		0x00000010
#define RES_ATTR_ECC_RESERVED_1			0x00000020
#define RES_ATTR_ECC_RESERVED_2			0x00000040
#define RES_ATTR_READ_PROTECTED			0x00000080
#define RES_ATTR_WRITE_PROTECTED		0x00000100
#define RES_ATTR_EXECUTION_PROTECTED		0x00000200
#define RES_ATTR_UNCACHEABLE			0x00000400
#define RES_ATTR_WRITE_COMBINEABLE		0x00000800
#define RES_ATTR_WRITE_THROUGH_CACHEABLE	0x00001000
#define RES_ATTR_WRITE_BACK_CACHEABLE		0x00002000
#define RES_ATTR_16_BIT_IO			0x00004000
#define RES_ATTR_32_BIT_IO			0x00008000
#define RES_ATTR_64_BIT_IO			0x00010000
#define RES_ATTR_UNCACHED_EXPORTED		0x00020000

/*
 * Describes the resource properties of all fixed, nonrelocatable resource
 * ranges found on the processor host bus during the HOB producer phase.
 */
struct hob_res_desc_t {
	struct hob_header_t	hdr;
	/*
	 * A GUID representing the owner of the resource. This GUID is
	 * used by HOB consumer phase components to correlate device
	 * ownership of a resource.
	 */
	struct efi_guid_t	owner;
	u32			type;
	u32			attr;
	/* The physical start address of the resource region */
	phys_addr_t		phys_start;
	/* The number of bytes of the resource region */
	phys_size_t		len;
};

/*
 * Allows writers of executable content in the HOB producer phase to
 * maintain and manage HOBs with specific GUID.
 */
struct hob_guid_t {
	struct hob_header_t	hdr;
	/* A GUID that defines the contents of this HOB */
	struct efi_guid_t	name;
	/* GUID specific data goes here */
};

/* Union of all the possible HOB Types */
union hob_pointers_t {
	struct hob_header_t	*hdr;
	struct hob_mem_alloc_t	*mem_alloc;
	struct hob_res_desc_t	*res_desc;
	struct hob_guid_t	*guid;
	u8			*raw;
};

/**
 * Returns the type of a HOB.
 *
 * This macro returns the type field from the HOB header for the
 * HOB specified by hob.
 *
 * @hob:    A pointer to a HOB.
 *
 * @return: HOB type.
 */
#define GET_HOB_TYPE(hob) \
	((*(struct hob_header_t **)&(hob))->type)

/**
 * Returns the length, in bytes, of a HOB.
 *
 * This macro returns the len field from the HOB header for the
 * HOB specified by hob.
 *
 * @hob:    A pointer to a HOB.
 *
 * @return: HOB length.
 */
#define GET_HOB_LENGTH(hob) \
	((*(struct hob_header_t **)&(hob))->len)

/**
 * Returns a pointer to the next HOB in the HOB list.
 *
 * This macro returns a pointer to HOB that follows the HOB specified by hob
 * in the HOB List.
 *
 * @hob:    A pointer to a HOB.
 *
 * @return: A pointer to the next HOB in the HOB list.
 */
#define GET_NEXT_HOB(hob)	\
	(void *)(*(u8 **)&(hob) + GET_HOB_LENGTH(hob))

/**
 * Determines if a HOB is the last HOB in the HOB list.
 *
 * This macro determine if the HOB specified by hob is the last HOB in the
 * HOB list.  If hob is last HOB in the HOB list, then TRUE is returned.
 * Otherwise, FALSE is returned.
 *
 * @hob:          A pointer to a HOB.
 *
 * @retval TRUE:  The HOB specified by hob is the last HOB in the HOB list.
 * @retval FALSE: The HOB specified by hob is not the last HOB in the HOB list.
 */
#define END_OF_HOB(hob)	(GET_HOB_TYPE(hob) == (u16)HOB_TYPE_EOH)

/**
 * Returns a pointer to data buffer from a HOB of type HOB_TYPE_GUID_EXT.
 *
 * This macro returns a pointer to the data buffer in a HOB specified by hob.
 * hob is assumed to be a HOB of type HOB_TYPE_GUID_EXT.
 *
 * @hob:    A pointer to a HOB.
 *
 * @return: A pointer to the data buffer in a HOB.
 */
#define GET_GUID_HOB_DATA(hob)	\
	(void *)(*(u8 **)&(hob) + sizeof(struct hob_guid_t))

/**
 * Returns the size of the data buffer from a HOB of type HOB_TYPE_GUID_EXT.
 *
 * This macro returns the size, in bytes, of the data buffer in a HOB
 * specified by hob. hob is assumed to be a HOB of type HOB_TYPE_GUID_EXT.
 *
 * @hob:    A pointer to a HOB.
 *
 * @return: The size of the data buffer.
 */
#define GET_GUID_HOB_DATA_SIZE(hob)	\
	(u16)(GET_HOB_LENGTH(hob) - sizeof(struct hob_guid_t))

/* FSP specific GUID HOB definitions */
#define FSP_HEADER_GUID \
	{ \
	0x912740be, 0x2284, 0x4734, \
	{0xb9, 0x71, 0x84, 0xb0, 0x27, 0x35, 0x3f, 0x0c} \
	}

#define FSP_NON_VOLATILE_STORAGE_HOB_GUID \
	{ \
	0x721acf02, 0x4d77, 0x4c2a, \
	{ 0xb3, 0xdc, 0x27, 0xb, 0x7b, 0xa9, 0xe4, 0xb0 } \
	}

#define FSP_BOOTLOADER_TEMP_MEM_HOB_GUID \
	{ \
	0xbbcff46c, 0xc8d3, 0x4113, \
	{ 0x89, 0x85, 0xb9, 0xd4, 0xf3, 0xb3, 0xf6, 0x4e } \
	}

#define FSP_HOB_RESOURCE_OWNER_FSP_GUID \
	{ \
	0x69a79759, 0x1373, 0x4367, \
	{ 0xa6, 0xc4, 0xc7, 0xf5, 0x9e, 0xfd, 0x98, 0x6e } \
	}

#define FSP_HOB_RESOURCE_OWNER_TSEG_GUID \
	{ \
	0xd038747c, 0xd00c, 0x4980, \
	{ 0xb3, 0x19, 0x49, 0x01, 0x99, 0xa4, 0x7d, 0x55 } \
	}

#define FSP_HOB_RESOURCE_OWNER_GRAPHICS_GUID \
	{ \
	0x9c7c3aa7, 0x5332, 0x4917, \
	{ 0x82, 0xb9, 0x56, 0xa5, 0xf3, 0xe6, 0x2a, 0x07 } \
	}

#endif
