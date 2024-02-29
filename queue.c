#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *queue = malloc(sizeof(struct list_head));
    if (!queue)
        return NULL;

    INIT_LIST_HEAD(queue);
    return queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head) {
        struct list_head *next = head->next;
        while (!list_empty(head)) {
            // cppcheck-suppress nullPointer
            element_t *ele = list_entry(next, element_t, list);
            list_del(next);
            next = next->next;
            if (ele) {
                if (ele->value)
                    free(ele->value);
                free(ele);
            }
        }

        free(head);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    int len = strlen(s);
    element_t *ele = malloc(sizeof(element_t));
    char *str = malloc(len + 1);
    if (!head || !ele || !str) {
        return false;
    }

    strncpy(str, s, len);
    str[len] = '\0';
    ele->value = str;
    list_add(&ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    int len = strlen(s);
    element_t *ele = malloc(sizeof(element_t));
    char *str = malloc(len + 1);
    if (!head || !ele || !str) {
        return false;
    }

    strncpy(str, s, len);
    str[len] = '\0';
    ele->value = str;
    list_add_tail(&ele->list, head);

    return true;
}

/* Remove an element from head of queue */

element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }

    // cppcheck-suppress nullPointer
    element_t *ele = list_first_entry(head, element_t, list);
    int len = strlen(ele->value);
    if (sp && len < bufsize - 1) {
        strncpy(sp, ele->value, len);
        sp[len] = '\0';
    }
    list_del(head->next);

    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }

    // cppcheck-suppress nullPointer
    element_t *ele = list_last_entry(head, element_t, list);
    int len = strlen(ele->value);
    if (sp && len < bufsize - 1) {
        strncpy(sp, ele->value, len);
        sp[len] = '\0';
    }
    list_del_init(head->prev);

    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (list_empty(head)) {
        return false;
    }

    struct list_head *fast = head->next;
    struct list_head *slow = head->next;

    if (list_is_singular(head) || fast->next->next == head) {
        list_del(head->next);
        return true;
    }

    while (fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_del(slow);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *first = head->next;
    struct list_head *second = first->next;
    bool is_del = false;

    while (first->next != head) {
        // cppcheck-suppress nullPointer
        element_t *ele1 = list_entry(first, element_t, list);
        // cppcheck-suppress nullPointer
        element_t *ele2 = list_entry(second, element_t, list);
        if (!strcmp(ele1->value, ele2->value)) {
            list_del(first);
            is_del = true;
        } else if (is_del) {
            list_del(first);
            is_del = false;
        }
        first = first->next;
        second = second->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!list_is_singular(head) && !list_empty(head)) {
        struct list_head *first = head->next;
        while (first->next != head) {
            struct list_head *second = first->next;
            struct list_head *fp = first->prev;
            struct list_head *sn = second->next;

            fp->next = second;
            sn->prev = first;

            first->prev = second;
            first->next = sn;

            second->prev = fp;
            second->next = first;

            first = first->next;
            if (first == head)
                break;
        }
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *next = head;
    do {
        struct list_head *tmp = next->next;
        next->next = next->prev;
        next->prev = tmp;
        next = next->prev;
    } while (next != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    //
    if (!head || list_empty(head)) {
        return 0;
    }
    int n = 1;
    int d_num = 0;
    struct list_head *cur = head->next;
    while (cur->next != head) {
        struct list_head *next = cur->next;
        // cppcheck-suppress nullPointer
        element_t *elec = list_entry(cur, element_t, list);
        // cppcheck-suppress nullPointer
        element_t *elen = list_entry(next, element_t, list);


        if (strcmp(elec->value, elen->value) > 0) {
            list_del(cur);
            struct list_head *prev = cur->prev;
            while (prev != head) {
                // cppcheck-suppress nullPointer
                element_t *elep = list_entry(prev, element_t, list);
                if (strcmp(elep->value, elen->value) > 0) {
                    list_del(prev);
                    d_num++;
                }
                prev = prev->prev;
            }
            d_num++;
        }

        cur = cur->next;
        n++;
        if (cur == head)
            break;
    }
    return n - d_num;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }
    int n = 1;
    int d_num = 0;
    struct list_head *cur = head->next;
    while (cur->next != head) {
        struct list_head *next = cur->next;
        // cppcheck-suppress nullPointer
        element_t *elec = list_entry(cur, element_t, list);
        // cppcheck-suppress nullPointer
        element_t *elen = list_entry(next, element_t, list);


        if (strcmp(elec->value, elen->value) < 0) {
            list_del(cur);
            struct list_head *prev = cur->prev;
            while (prev != head) {
                // cppcheck-suppress nullPointer
                element_t *elep = list_entry(prev, element_t, list);
                if (strcmp(elep->value, elen->value) < 0) {
                    list_del(prev);
                    d_num++;
                }
                prev = prev->prev;
            }
            d_num++;
        }

        cur = cur->next;
        n++;
        if (cur == head)
            break;
    }
    return n - d_num;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
