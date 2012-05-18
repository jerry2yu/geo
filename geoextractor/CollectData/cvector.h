/** 
 * File: cvector.h
 * ---------------
 * Defines the interface for the vector.
 *
 * The cvector allows the client to store any number of elements of any desired
 * primitive type and is appropriate for a wide variety of storage problems.  It 
 * supports efficient element access and appending/inserting/deleting elements
 * as well as optional sorting and searching.  In all cases, the cvector imposes 
 * no upper bound on the number of elements and deals with all its own memory 
 * management. The client specifies the size (in bytes) of the elements that 
 * will be stored in the cvector when it is created.  Thereafter the client and 
 * the cvector can refer to elements via (void *) ptrs.
 */

#ifndef _cvector_
#define _cvector_
#include <assert.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#define DEFAULT_VECTOR_SIZE 521 /*default size of cvector if client passes 0 for initialAllocation when call VectorNew*/

/**
 * Type: VectorCompareFunction
 * ---------------------------
 * A callback function is an argument to a function that allows 
 * a application to specify a function that is required to be called. 
 * This argument is what is called a "function pointer", 
 * in that it is a pointer to a function. unlike other pointers in C, 
 * you don't have to specify "&" to get the function pointer, just the function name.
 * VectorCompareFunction is a pointer to a client-supplied function which the
 * cvector uses to sort or search for elements.  The comparator takes two 
 * (const void *) pointers (these will point to elements) and returns an int.
 * The comparator should indicate the ordering of the two elements
 * using the same convention as the strcmp library function:
 * 
 *   If elemAddr1 is less than elemAddr2, return a negative number.
 *   If elemAddr2 is greater than elemAddr2, return a positive number.
 *   If the two elements are equal, return 0.
 */

typedef int (*VectorCompareFunction)(const void *elemAddr1, const void *elemAddr2);

/** 
 * Type: VectorMapFunction
 * -----------------------
 * VectorMapFunction defines the space of functions that can be used to map over
 * the elements in a vector.  A map function is called with a pointer to
 * the element and a client data pointer passed in from the original
 * caller.
 */

typedef void (*VectorMapFunction)(void *elemAddr, void *auxData);

 
/** 
 * Type: VectorFreeFunction
 * ---------------------------------
 * VectorFreeFunction defines the space of functions that can be used as the
 * clean-up function for each element as it is deleted from the vector
 * or when the entire cvector is destroyed.  The cleanup function is 
 * called with a pointer to an element about to be deleted.
 */

typedef void (*VectorFreeFunction)(void *elemAddr);

/**
 * Type: vector
 * ------------
 * Defines the concrete representation of
 * the vector.  Even though everything is
 * exposed, the client should respect the 
 * the privacy of the representation and initialize,
 * dispose of, and otherwise interact with a
 * cvector using those functions defined in this file.
 */

typedef struct cvector
{
	// implementation-specific…for you to decide
	int ItemSize; /* size of one element */
	int InitCount; /* the initial allocated memory space for elements */
	int ArrayCount;/* the total allocated memory space for elements */
	long ItemsCount; /*total number of elements in the vectory*/
	void* Items; /* pointer to actual element */
	VectorFreeFunction ItemFreeFunction; /* callback function to free element */
}  cvector;

/** 
 * Function: VectorNew
 * Usage: cvector myFriends;
 *        VectorNew(&myFriends, sizeof(char *), StringFree, 10);
 * -------------------
 * Constructs a raw or previously destroyed cvector to be the
 * empty vector.
 * 
 * The elemSize parameter specifies the number of bytes that a single 
 * element of the cvector should take up.  For example, if you want to store 
 * elements of type char *, you would pass sizeof(char *) as this parameter.
 * An assert is raised if the size is not greater than zero.
 *
 * The ArrayFreeFunction is the function that will be called on an element that
 * is about to be deleted (using VectorDelete) or on each element in the
 * cvector when the entire cvector is being freed (using VectorDispose).  This function
 * is your chance to do any deallocation/cleanup required for the element
 * (such as freeing/deleting any pointers contained in the element). The client can pass 
 * NULL for the ArrayFreeFunction if the elements don't require any special handling.
 *
 * The initialAllocation parameter specifies the initial allocated length of the 
 * vector, as well as the dynamic reallocation increment for those times when the 
 * cvector needs to grow.  Rather than growing the cvector one element at a time as 
 * elements are added (inefficient), you will grow the cvector 
 * in chunks of initialAllocation size.  The allocated length is the number
 * of elements for which space has been allocated: the logical length 
 * is the number of those slots currently being used.
 * 
 * A new cvector pre-allocates space for initialAllocation elements, but the
 * logical length is zero.  As elements are added, those allocated slots fill
 * up, and when the initial allocation is all used, grow the cvector by another 
 * initialAllocation elements.  You will continue growing the cvector in chunks
 * like this as needed.  Thus the allocated length will always be a multiple
 * of initialAllocation.  Don't worry about using realloc to shrink the vector's 
 * allocation if a bunch of elements get deleted.  It turns out that 
 * many implementations of realloc don't even pay attention to such a request, 
 * so there is little point in asking.  Just leave the cvector over-allocated and no
 * one will care.
 *
 * The initialAllocation is the client's opportunity to tune the resizing
 * behavior for his/her particular needs.  Clients who expect their vectors to
 * become large should probably choose a large initial allocation size, whereas
 * clients who expect the cvector to be relatively small should choose a smaller
 * initialAllocation size.  You want to minimize the number of reallocations, but
 * you don't want to pre-allocate all that much memory if you don't expect to use very
 * much of it.  If the client passes 0 for initialAllocation, the implementation
 * will use the default value of its own choosing.  As assert is raised is 
 * the initialAllocation value is less than 0.
 */

void VectorNew (struct cvector *v, int elemSize, VectorFreeFunction freefn, int initialAllocation);

/**
 * Function: VectorDispose
 *           VectorDispose(&studentsDroppingTheCourse);
 * -----------------------
 * Frees up all the memory of the specified cvector and its elements.  It does *not* 
 * automatically free memory owned by pointers embedded in the elements. 
 * This would require knowledge of the structure of the elements, which the 
 * cvector does not have.  However, it *will* iterate over the elements calling
 * the VectorFreeFunction previously supplied to VectorNew.
 */

void VectorDispose (cvector *v);

/**
 * Function: VectorLength
 * ----------------------
 * Returns the logical length of the vector, i.e. the number of elements
 * currently in the vector.  Must run in constant time.
 */

int VectorLength (const cvector *v);
	   
/**
 * Method: VectorNth
 * -----------------
 * Returns a pointer to the element numbered position in the vector.  
 * Numbering begins with 0.  An assert is raised if n is less than 0 or greater 
 * than the logical length minus 1.  Note this function returns a pointer into 
 * the vector's storage, so the pointer should be used with care.
 * This method must operate in constant time.
 *
 * We could have written the cvector without this sort of access, but it
 * is useful and efficient to offer it, although the client needs to be 
 * careful when using it.  In particular, a pointer returned by VectorNth 
 * becomes invalid after any calls which involve insertion into, deletion from or 
 * sorting of the vector, as all of these may rearrange the elements to some extent.
 */ 

void *VectorNth (const cvector *v, int position);
					  
/**
 * Function: VectorInsert
 * ----------------------
 * Inserts an element into the specified vector, placing it at the specified position.
 * An assert is raised if n is less than 0 or greater than the logical length.
 * The cvector elements after the supplied position will be shifted over to make room. 
 * The element is passed by address: The new element's contents are copied from 
 * the memory pointed to by elemAddr.  This method runs in linear time.
 */

void VectorInsert (cvector *v, const void *elemAddr, int position);

/**
 * Function: VectorAppend
 * ----------------------
 * Appends a new element to the end of the specified vector.  The element is 
 * passed by address, and the element contents are copied from the memory pointed 
 * to by elemAddr.  Note that right after this call, the new element will be 
 * the last in the vector; i.e. its element number will be the logical length 
 * minus 1.  This method must run in constant time (neglecting the memory reallocation 
 * time which may be required occasionally).
 */

void VectorAppend (cvector *v, const void *elemAddr);

/**
 * Function: VectorReplace
 * -----------------------
 * Overwrites the element at the specified position with a new value.  Before 
 * being overwritten, the VectorFreeFunction that was supplied to VectorNew is levied
 * against the old element.  Then that position in the cvector will get a new value by
 * copying the new element's contents from the memory pointed to by elemAddr.
 * An assert is raised if n is less than 0 or greater than the logical length 
 * minus one.  None of the other elements are affected or rearranged by this
 * operation, and the size of the cvector remains constant. This method must
 * operate in constant time.
 */

void VectorReplace (cvector *v, const void *elemAddr, int position);

/**
 * Function: VectorDelete
 * ----------------------
 * Deletes the element at the specified position from the vector. Before the 
 * element is removed,  the ArrayFreeFunction that was supplied to VectorNew
 * will be called on the element.
 *
 * An assert is raised if position is less than 0 or greater than the logical length 
 * minus one.  All the elements after the specified position will be shifted over to
 * fill the gap.  This method runs in linear time.  It does not shrink the 
 * allocated size of the cvector when an element is deleted; the cvector just 
 * stays over-allocated.
 */

void VectorDelete (cvector *v, int position);

 
/** 
 * Function: VectorSearch
 * ----------------------
 * Searches the specified cvector for an element whose contents match
 * the element passed as the key.  Uses the comparator argument to test
 * for equality.  The startIndex parameter controls where the search
 * starts.  If the client desires to search the entire vector,
 * they should pass 0 as the startIndex.  The method will search from
 * there to the end of the vector.  The isSorted parameter allows the client 
 * to specify that the cvector is already in sorted order, in which case VectorSearch
 * uses a faster binary search.  If isSorted is false, a simple linear search is 
 * used.  If a match is found, the position of the matching element is returned;
 * else the function returns -1.  Calling this function does not 
 * re-arrange or change contents of the cvector or modify the key in any way.
 * 
 * An assert is raised if startIndex is less than 0 or greater than
 * the logical length (although searching from logical length will never
 * find anything, allowing this case means you can search an entirely empty
 * cvector from 0 without getting an assert).  An assert is raised if the
 * comparator or the key is NULL.
 */  

int VectorSearch ( const cvector *v, const void *key, VectorCompareFunction searchfn, 
			     int startIndex, bool isSorted );

/**
 * Function: VectorSort
 * --------------------
 * Sorts the cvector into ascending order according to the supplied
 * comparator.  The numbering of the elements will change to reflect the 
 * new ordering.  An assert is raised if the comparator is NULL.
 */

void VectorSort (cvector *v, VectorCompareFunction comparefn);

/**
 * Method: VectorMap
 * -----------------
 * Iterates over the elements of the cvector in order (from element 0 to
 * element n-1, inclusive) and calls the function mapfn on each element.  The function
 * is called with the address of the cvector element and the auxData pointer.  
 * The auxData value allows the client to pass extra state information to 
 * the client-supplied function, if necessary.  If no client data is required, 
 * this argument should be NULL.  An assert is raised if the mapfn function is NULL.
 */

void VectorMap (cvector *v, VectorMapFunction mapfn, void *auxData);

#endif

