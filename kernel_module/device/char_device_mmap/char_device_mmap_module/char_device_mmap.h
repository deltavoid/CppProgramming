#ifndef CHAR_DEVICE_MMAP_H
#define CHAR_DEVICE_MMAP_H



#define EXAMPLE_CHAR_DEVICE_PATH "/dev/example_device"


#define EXAMPLE_CHAR_DEVICE_QUEUE_SIZE (64 * 1024 * 1024)
// 64MB



struct example_mmap_char_dev_ctrl_t {
    // int queue_chunk_size;
    // int queue_chunk_num_bit;
    // int queue_size;
    // int tmp;
    // uint64_t tx_id;
    int queue_size;
};

struct example_mmap_char_dev_meta_t {
    struct example_mmap_char_dev_ctrl_t* ctrl;
    void* queue;
};


#define EXAMPLE_CHAR_DEVICE_MMAP_CTRL_OFFSET 0x0000
#define EXAMPLE_CHAR_DEVICE_MMAP_QUEUE_OFFSET 0x1000

#define EXAMPLE_CHAR_DEVICE_MMAP_CTRL_SIZE sizeof(struct example_mmap_char_dev_ctrl_t)
#define EXAMPLE_CHAR_DEVICE_MMAP_QUEUE_SIZE EXAMPLE_CHAR_DEVICE_QUEUE_SIZE














#endif // CHAR_DEVICE_MMAP_H
