/**
 * Using segregated free list.
 * Optimize boundary tags.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "malloc during military training",
    /* First member's full name */
    "sean",
    /* First member's email address */
    "seanxunx@outlook.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/*
 * The structure of a free block
 * --- header (4 bytes) ----
 * --- next_ptr (4 bytes) ---
 * --- prev_ptr (4 bytes) ---
 * --- payload (some bytes) ---
 * --- footer (4 bytes) ---
 */

/**
 * The structure of heap:
 *      seglist      
 *      block
 */

#define MAXNUM (1 << 21)
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

// My basic constants and macros

#define WSIZE 4             // Word and header/footer size (bytes)
#define DSIZE 8             // Double word size
#define CHUNKSIZE (1 << 12) // Extend heap by this amount
#define MIN_BLOCK_SIZE 16   // header + footer + prev_ptr + next_ptr (size of ptr is 4 bytes)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// Read the size and allocated fields from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PREVALLOC(p) (GET(p) & 0x2)
#define GET_SIGNBITS(p) (GET(p) & 0x7)

// Set the prev alloc bit
#define SET_PREVALLOC(p) (PUT(p, PACK(GET(p), 0x2)))
#define SET_PRENOTALLOC(p) (PUT(p, (GET(p) & ~0x2)))

// Get the pointer to the next/previous block
#define GET_NEXTPTR(bp) GET((char *)(bp))
#define GET_PREVPTR(bp) (GET((char *)(bp) + WSIZE))

// Put the pointers
#define PUT_NEXTPTR(bp, next_ptr) (PUT(bp, next_ptr))
#define PUT_PREVPTR(bp, prev_ptr) (PUT(((char *)(bp) + WSIZE), prev_ptr))

// Given block ptr bp, compute address of its header and footer
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

// Given block ptr bp, compute address of next and previous blocks
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))


static void *heap_listp;
static void **seglist_ptr;
// static void *free_listp;
static size_t heap_size;

/**
 * Calculate the power of 2.
 */
static size_t cal_pw2(size_t size) {
    size_t pw = 0;
    size /= 2;
    while (size) {
        ++pw;
        size /= 2;
    }
    return pw;
}

/**
 * Get the index of seglist with given bp
 */
static size_t get_index(void *bp) {
    size_t size = GET_SIZE(HDRP(bp));
    if (size <= 0) {
        return -1;
    }
    return cal_pw2(size);
} 

/**
 * Get the free block's own free_listp.
 */
static void* get_free_listp(void *bp) {
    return seglist_ptr[get_index(bp)];
}

/**
 * Get free_listp with size.
 */
static void* get_free_listp_size(size_t size) {
    return seglist_ptr[cal_pw2(size)];
}

/**
 * Set the free_listp as bp.
 */
static void set_free_listp(void *bp, void *new_ptr) {
    if (bp != NULL) {
        seglist_ptr[get_index(bp)] = new_ptr;
    }
}

/**
 * isValidBp - checks if the block that bp points to is valid
 * If the size of the block is zero, then it is the epilogue.
 * If the address bp is less than heap_listp, then it is out of bound.
 */
static int isValidBp(void *bp) {
    if (bp < heap_listp - WSIZE || GET_SIZE(HDRP(bp)) == 0) {
        return 0;
    }
    return 1;
}

static int hirachy_num = 0;
/**
 * mm_hirachy - visionalize the current heap and free list
 */
static void mm_hirachy() {
    printf("------------------------------------\n");
    printf("hirachy num = %d\n", hirachy_num);
    /**
     * prints all the blocks as follows:
     *      
     *      - block 1 (0x1234568)
     *      |    - header/footer: 0x12345678
     *      |    - next_ptr: 0x12345678
     *      |    - prev_ptr: 0x12345678
     * 
     */
    int index = 1, size = 0;
    void *p = heap_listp;
    // int free_indexes[MAXNUM];
    printf("\n*** HEAP HIRACHY ***\n");
    while (isValidBp(p)) {
        // if (!GET_ALLOC(p)) {
        //     free_indexes[size++] = index;
        // }
        printf("\t*-block %d (%p)\n", index++, p);
        printf("\t\t|\theader/footer: %x\n", GET(HDRP(p)));
        printf("\t\t|\tsize = %d, is_allocated = %d \n", 
               GET_SIZE(HDRP(p)), GET_ALLOC(HDRP(p)));
        printf("\t\t|\tnext_ptr: %p\n", GET_NEXTPTR(p));
        printf("\t\t|\tprev_ptr: %p\n", GET_PREVPTR(p));
        p = NEXT_BLKP(p);
    }
    
    /**
     * prints the seglist
     */
    printf("\n*** Seglist ***\n");
    for (int i = 0; i < 26; ++i) {
        if (seglist_ptr[i]) {
            printf("\t2^%d ~ 2^%d: %p\n", i, i + 1, seglist_ptr[i]);
        }
    }
    printf("------------------------------------\n");
}

/**
 * Checks if every block in the free list marked as free and valid.
 */
// static int check_free_list() {
//     void *ptr = free_listp;
//     while (ptr != NULL) {
//         if (GET_ALLOC(ptr) || !isValidBp(ptr)) {
//             // if the block is allocated
//             return 0;
//         }
//        ptr = GET_NEXTPTR(ptr);
//     }
//     return 1;
// }

/**
 * check_is_coalesced - checks if any contiguous free blocks are left not coalesced
 * If continuous free blocks exist, then return the pointer pointing to the first one.
 * Otherwise, returns NULL.
//  */
// static void* check_is_coalesced() {
//     void *ptr = free_listp, *next_ptr, *next_bp;
//     while (GET_NEXTPTR(ptr) != NULL && NEXT_BLKP(ptr) != NULL) {
//         next_ptr = GET_NEXTPTR(ptr);
//         next_bp = NEXT_BLKP(ptr);
//         if (next_bp == next_ptr) {
//             return ptr;
//         }
//         ptr = next_ptr;
//     }
//     return NULL;
// }

/**
 * is_contained - if the free block list contains bp
 */
// static int is_contained(void* bp) {
//     void *p = free_listp;
//     while (p) {
//         if (p == bp) {
//             return 1;
//         }
//         p = GET_NEXTPTR(p);
//     }
//     return 0;
// }

/**
 * is_all_contained - checks if every free block is in the free list
 */
// static int is_all_contained() {
//     void *bp = heap_listp;
//     while (isValidBp(bp)) {
//         if (!GET_ALLOC(bp) && !is_contained(bp)) {
//             return 0;
//         }
//         bp = NEXT_BLKP(bp);
//     }
//     return 1;
// }

/**
 * mm_check - heap consistency checker
 *  help with debugging and print out the hirachy of the current
 *  heap and free list
 *  returns a non-zero value if and only if the heap is consistent
 *
 *  Check List:
 *  1. Is every block in the free list marked as free?
 *  2. Are there any contiguous free blocks that somehow escaped
 *  coalescing?
 *  3. Is every free block actually in the free list?
 *  4. Do the pointers in the free list point to valid free blocks?
 *  5. Do any allocated blocks overlap?
 *  6. Do the pointers in a heap block pint to valid addresses?
 *
 *  What's more, call the mm_hirachy function to draw the hirachy 
 *  visionally.
 *
 *  Remeber to remove any calls to mm_check!!!
//  */ 
// static int mm_check() {
//     if (!check_free_list()) {
//         printf("ERROR: check_free_list\n");
//         mm_hirachy();
//         exit(1);
//     } else if (check_is_coalesced()) {
//         printf("ERROR: check_is_coalesced\n");
//         mm_hirachy();
//         exit(1);
//     } else if (!is_all_contained()) {
//         printf("ERROR: is_all_contained\n");
//         mm_hirachy();
//         exit(1);
//     }
//     return 1;
// }

/**
 * insert_block - inserts the given block at the head of free list
 */
static void insert_block(void *bp)
{
    // Last in first out
    // LIFO
    void *free_listp = get_free_listp(bp); 
    PUT_NEXTPTR(bp, free_listp);
    PUT_PREVPTR(bp, NULL);
    if (free_listp != NULL) {
        PUT_PREVPTR(free_listp, bp);
    }
    set_free_listp(bp, bp);
}

/**
 * remove_block - removes the given block from the free block list
 */
static void remove_block(void *bp)
{
    void *next_bp = GET_NEXTPTR(bp);
    void *prev_bp = GET_PREVPTR(bp);
    if (prev_bp != NULL) {
        PUT_NEXTPTR(prev_bp, next_bp);
    }
    if (next_bp != NULL) {
        PUT_PREVPTR(next_bp, prev_bp);
    }
    if (prev_bp == NULL) {
        // deleting the first entity in the free list
        set_free_listp(bp, next_bp);
    } 
    
}

/**
 * coalesce - coalesce the free blocks
 */
static void *coalesce(void *bp)
{
    void *prev_bp = PREV_BLKP(bp), *next_bp = NEXT_BLKP(bp);
    size_t next_alloc, prev_alloc;
    // size_t prev_alloc = GET_PREVALLOC(HDRP(bp));
    prev_alloc = GET_PREVALLOC(HDRP(bp));
    if (isValidBp(next_bp)) {
        next_alloc = GET_ALLOC(HDRP(next_bp));
    } else {
        next_alloc = 1;
    }

    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
    {
    }
    else if (prev_alloc && !next_alloc)
    {
        // only next block is free, previous block is allocated
        remove_block(next_bp);
        size += GET_SIZE(HDRP(next_bp));
        PUT(HDRP(bp), PACK(size, 2));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc)
    {
        // only previous block is free
        remove_block(prev_bp);
        size += GET_SIZE(FTRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, GET_PREVALLOC(HDRP(PREV_BLKP(bp)))));
        bp = PREV_BLKP(bp);
    }
    else
    {
        // both next and previous blocks are free
        remove_block(prev_bp);
        remove_block(next_bp);
        size += GET_SIZE(HDRP(next_bp)) + GET_SIZE(FTRP(prev_bp));
        PUT(HDRP(prev_bp), PACK(size, GET_PREVALLOC(HDRP(prev_bp))));
        PUT(FTRP(next_bp), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    insert_block(bp);
    return bp;
}

/**
 * extend_heap - extends the heap with a new free block
 */
static void *extend_heap(size_t asize)
{
    char *bp;
    size_t size, words;
    // Allocates an even number of words to maintain alignment
    words = asize % WSIZE ? asize / WSIZE + 1 : asize / WSIZE;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    heap_size += size;
    if ((long)(bp = mem_sbrk(size)) == -1)
    {
        return NULL;
    }

    size_t is_pre_alloc = GET_PREVALLOC(HDRP(bp));
    // Initialize free block header/footer and the epilogue header
    PUT(HDRP(bp), PACK(size, is_pre_alloc));         // Free block header
    PUT(FTRP(bp), PACK(size, 0));         // Free block footer
    PUT_NEXTPTR(bp, NULL);
    PUT_PREVPTR(bp, NULL);
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // New epilogue header

    // Coalesce if any adjacent blocks is free
    return coalesce(bp);
}

/**
 * find_fit - Performs a first-fit search of the implicit free list.
 */
static void *find_fit(size_t asize)
{
    void *tmp;
    size_t tmp_size, index = cal_pw2(asize);
    while (index < 26) {
        tmp = seglist_ptr[index];
        while (tmp != NULL)
        {
            tmp_size = GET_SIZE(HDRP(tmp));
            if (!GET_ALLOC(HDRP(tmp)) && tmp_size + WSIZE >= asize)
            {
                return tmp;
            }
            tmp = GET_NEXTPTR(tmp);
        }
        ++index;
    }
    return NULL;
}


/**
 * place - places the requested block at the beginning of the free
 *      block, spliting only if the size remainder would equal or
 *      exceed the minmum block size.
 */
static void place(void *bp, size_t asize)
{
    remove_block(bp);
    size_t now_size = GET_SIZE(HDRP(bp));
    size_t remainder = now_size - asize;
    if (remainder >= MIN_BLOCK_SIZE)
    {
        // split
        PUT(HDRP(bp), PACK(asize, GET_PREVALLOC(HDRP(bp)) | 0x1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(remainder, 0));
        PUT(FTRP(bp), PACK(remainder, 0));
        SET_PREVALLOC(HDRP(bp));
        insert_block(bp);
    }
    else
    {
        PUT(HDRP(bp), PACK(now_size, GET_PREVALLOC(HDRP(bp)) | 0x1));
        PUT(FTRP(bp), PACK(now_size, 1));
        // remove_block(bp);
        void *next_bp = NEXT_BLKP(bp);
        SET_PREVALLOC(HDRP(next_bp));
    }
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    // Creates the initial empty heap
    if ((seglist_ptr = mem_sbrk((4 + 26) * WSIZE)) == (void *)-1) // 3 is for alignment
    {
        return -1;
    }
    for (int i = 0; i < 26; ++i) {
        // initialize setlist
        PUT(seglist_ptr + i, 0);
    }
    heap_listp = seglist_ptr + 26;
    PUT(heap_listp, 0);                            // Alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); // Prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); // Prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 0b11));     // Epilogue header
    heap_listp += (2 * WSIZE);

    // Extend the empty heap with a free block of CHUNKSIZE bytes
    if ((heap_listp = extend_heap(CHUNKSIZE)) == NULL)
    {
        return -1;
    }
    PUT_NEXTPTR(heap_listp, NULL);
    PUT_PREVPTR(heap_listp, NULL);
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

    size_t asize;      // adjusted block size
    size_t extendsize; // amount to extend heap if no fit
    char *bp;

    // Ignore spurious requests
    if (size <= 0)
    {
        return NULL;
    }

    // Adjust block size to include overhead and alignment requirests
    asize = (2 + size / DSIZE + (size % DSIZE ? 1 : 0)) * DSIZE;

    // Search the free list for a fit
    if ((bp = find_fit(asize)) != NULL)
    {
        place(bp, asize);

    ++hirachy_num;
    //   // TODO: delete hirachy
    //   mm_hirachy();
        return bp;
    }

    // No fit found. Get more memory and place the block.
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL)
    {
        return NULL;
    }
    place(bp, asize);

    ++hirachy_num;
    //   // TODO: delete hirachy
    //   mm_hirachy();
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{

    size_t size = GET_SIZE(HDRP(ptr));
    size_t is_pre_alloc = GET_PREVALLOC(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, is_pre_alloc));
    PUT(FTRP(ptr), PACK(size, 0));
    void *next_bp = NEXT_BLKP(ptr);
    SET_PRENOTALLOC(HDRP(next_bp));
    coalesce(ptr);

    ++hirachy_num;
    //   // TODO: delete hirachy
    //   mm_hirachy();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    // copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    copySize = GET_SIZE(HDRP(oldptr)) - DSIZE;
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
