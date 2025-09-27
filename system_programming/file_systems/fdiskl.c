#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#define ONE_GB                 (1024 * 1024 * 1024)
#define SECTOR_SIZE            (512)
#define MBR_SIZE               (512)
#define NO_EXTENDED            (0)
#define PARTITION_TABLE_START  (446)
#define ERROR                  (1)
#define SUCCESS                (0)
#define DISK_DEVICE            argv[1]
#define MAGIC_NUMBER           (0xAA55)
#define MAGIC_NUMBER_POS       (510)
#define LOGIC_START            (3)

typedef enum{
    PARTITION_TYPE_EXTENDED = 0x05,
    PARTITION_TYPE_NTFS     = 0x07,
    PARTITION_TYPE_FAT32    = 0x0B,
    PARTITION_TYPE_LINUX    = 0x83
}partition_types;

typedef enum{
    UNBOOTABLE = 0x00, BOOTABLE = 0x80
}partition_boot;

typedef struct {
    uint8_t   status;
    uint8_t   first_chs[3];
    uint8_t   type;
    uint8_t   last_chs[3];
    uint32_t  lba;
    uint32_t  size;
} PartitionEntry;

int main(int argc, char **argv)
{
    uint8_t  buf[MBR_SIZE];
    uint32_t base_extended = NO_EXTENDED;
    uint32_t next_lba = 0;
    uint64_t extended_lba = 0;
    uint8_t  id = 0;

    /* Open disk device (need root privileges) */
    int fd = open(DISK_DEVICE, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening disk");
        return ERROR;
    }

    /* Read first 512 bytes (MBR) */
    if (read(fd, buf, MBR_SIZE) != MBR_SIZE)
    {
        perror("Error reading buf");
        close(fd);
        return ERROR;
    }

    /* Check MBR signature */
    if( *((uint16_t*)&buf[MAGIC_NUMBER_POS]) != MAGIC_NUMBER )
    {
        printf("Invalid buf signature!\n");
        return ERROR;
    }

    /* Saving Partition Table Data */
    PartitionEntry* partition_table = (PartitionEntry*) &buf[PARTITION_TABLE_START];

    /* Printing Data */
    printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s %-15s\n", 
        "Device", "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

    for (id = 0; id < 4; id++)
    {
        /* Check if this partition is not used */
        if(partition_table[id].type == 0)
        {
            continue;
        }

        /* Check if there is an extended partition */
        if(partition_table[id].type == PARTITION_TYPE_EXTENDED)
        {
            base_extended = partition_table[id].lba;
        }

	    printf("%s%-7d %-15s %-15u %-15u %-15u %-6.2f%-9c %-15X %s\n",
            DISK_DEVICE,
	        id + 1,
	        (partition_table[id].status == BOOTABLE)?"[*]":"[ ]",
	        partition_table[id].lba,
            partition_table[id].lba + partition_table[id].size - 1,
            partition_table[id].size,
            (float)(((double) partition_table[id].size * SECTOR_SIZE) / ONE_GB),
            'G',
            partition_table[id].type,
            (partition_table[id].type == PARTITION_TYPE_EXTENDED) ? "Extended":
            (partition_table[id].type == PARTITION_TYPE_NTFS)     ? "NTFS/exFAT":
            (partition_table[id].type == PARTITION_TYPE_FAT32)    ? "FAT32":
            (partition_table[id].type == PARTITION_TYPE_LINUX)    ? "Linux":
            "Unknown / Reserved");
    }

    /* Printing the logic partitions if exists */
    if(base_extended)
    {
        /* Getting the first EBR */
        extended_lba = (uint64_t)base_extended * SECTOR_SIZE;
        next_lba = base_extended;
        lseek(fd, extended_lba, SEEK_SET);
        
        /* Read first 512 bytes (First EBR) */
        if (read(fd, buf, MBR_SIZE) != MBR_SIZE)
        {
            perror("Error reading buf");
            close(fd);
            return ERROR;
        }

        /* Check EBR signature */
        if( *((uint16_t*)&buf[MAGIC_NUMBER_POS]) != MAGIC_NUMBER )
        {
            printf("Invalid buf signature!\n");
            return ERROR;
        }

        /* Saving Partition Table Data */
        partition_table = (PartitionEntry*) &buf[PARTITION_TABLE_START];

        id = LOGIC_START;
        for(;;)
        {
            ++id;
            printf("%s%-7d %-15s %-15u %-15u %-15u %-6.2f%-9c %-15X %s\n",
                DISK_DEVICE,
	            id + 1,
	            (partition_table[0].status == BOOTABLE)?"[*]":"[ ]",
	            partition_table[0].lba + next_lba,
                partition_table[0].lba + next_lba + partition_table[0].size - 1,
                partition_table[0].size,
                (float)(((double) partition_table[0].size * SECTOR_SIZE) / ONE_GB),
                'G',
                partition_table[0].type,
                (partition_table[0].type == PARTITION_TYPE_EXTENDED) ? "Extended":
                (partition_table[0].type == PARTITION_TYPE_NTFS)     ? "NTFS/exFAT":
                (partition_table[0].type == PARTITION_TYPE_FAT32)    ? "FAT32":
                (partition_table[0].type == PARTITION_TYPE_LINUX)    ? "Linux":
                "Unknown / Reserved");

            /* If no more logic partitions, break */
            if(*((uint16_t*)(partition_table + 1)) == 0)
            {
                break;
            }

            /* Getting the Next EBR */
            next_lba = base_extended + partition_table[1].lba;
            extended_lba = (uint64_t)next_lba * SECTOR_SIZE;
            lseek(fd, extended_lba, SEEK_SET);
            
            /* Read first 512 bytes (First EBR) */
            if (read(fd, buf, MBR_SIZE) != MBR_SIZE)
            {
                perror("Error reading buf");
                close(fd);
                return ERROR;
            }

            /* Check EBR signature */
            if( *((uint16_t*)&buf[MAGIC_NUMBER_POS]) != MAGIC_NUMBER )
            {
                printf("Invalid buf signature!\n");
                return ERROR;
            }

            /* Saving Partition Table Data */
            partition_table = (PartitionEntry*) &buf[PARTITION_TABLE_START];
        }
    }

    close(fd);
    return SUCCESS;
}
