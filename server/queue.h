/*
 * Copyright (c) 1991, 1993
 * The Regents of the University of California. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Modified: May 15, 2013
 * Author: Rogério Carvalho Schneider <stockrt@gmail.com>
 *
 * Mainly indentation, some new features, automatic feature table update, and
 * working samples with tests.
 */

 #ifndef _SYS_QUEUE_H_
 #define _SYS_QUEUE_H_
 
 /*
  * This file defines five types of data structures: singly-linked lists,
  * lists, singly-linked tail queues, tail queues, and circular queues.
  *
  * A singly-linked list is headed by a single forward pointer. The elements
  * are singly linked for minimum space and pointer manipulation overhead at
  * the expense of O(n) removal for arbitrary elements. New elements can be
  * added to the list after an existing element or at the head of the list.
  * Elements being removed from the head of the list should use the explicit
  * macro for this purpose for optimum efficiency. A singly-linked list may
  * only be traversed in the forward direction. Singly-linked lists are ideal
  * for applications with large datasets and few or no removals or for
  * implementing a LIFO queue.
  *
  * A list is headed by a single forward pointer (or an array of forward
  * pointers for a hash table header). The elements are doubly linked
  * so that an arbitrary element can be removed without a need to
  * traverse the list. New elements can be added to the list before
  * or after an existing element or at the head of the list. A list
  * may only be traversed in the forward direction.
  *
  * A singly-linked tail queue is headed by a pair of pointers, one to the
  * head of the list and the other to the tail of the list. The elements are
  * singly linked for minimum space and pointer manipulation overhead at the
  * expense of O(n) removal for arbitrary elements. New elements can be added
  * to the list after an existing element, at the head of the list, or at the
  * end of the list. Elements being removed from the head of the tail queue
  * should use the explicit macro for this purpose for optimum efficiency.
  * A singly-linked tail queue may only be traversed in the forward direction.
  * Singly-linked tail queues are ideal for applications with large datasets
  * and few or no removals or for implementing a FIFO queue.
  *
  * A tail queue is headed by a pair of pointers, one to the head of the
  * list and the other to the tail of the list. The elements are doubly
  * linked so that an arbitrary element can be removed without a need to
  * traverse the list. New elements can be added to the list before or
  * after an existing element, at the head of the list, or at the end of
  * the list. A tail queue may be traversed in either direction.
  *
  * A circle queue is headed by a pair of pointers, one to the head of the
  * list and the other to the tail of the list. The elements are doubly
  * linked so that an arbitrary element can be removed without a need to
  * traverse the list. New elements can be added to the list before or after
  * an existing element, at the head of the list, or at the end of the list.
  * A circle queue may be traversed in either direction, but has a more
  * complex end of list detection.
  *
  * For details on the use of these macros, see the queue(3) manual page.
  *
  *                        SLIST    LIST   STAILQ  TAILQ  CIRCLEQ
  *
  * _HEAD                    +       +       +       +       +
  * _HEAD_INITIALIZER        +       +       +       +       +
  * _ENTRY                   +       +       +       +       +
  *
  * _INIT                    +       +       +       +       +
  * _INSERT_HEAD             +       +       +       +       +
  * _INSERT_TAIL             -       -       +       +       +
  * _INSERT_BEFORE           -       +       -       +       +
  * _INSERT_AFTER            +       +       +       +       +
  * _CONCAT                  -       -       +       +       -
  * _REMOVE_HEAD             +       -       +       -       -
  * _REMOVE_HEAD_UNTIL       -       -       +       -       -
  * _REMOVE_TAIL             -       -       -       -       -
  * _REMOVE_TAIL_UNTIL       -       -       -       -       -
  * _REMOVE_BEFORE           -       -       -       -       -
  * _REMOVE_AFTER            +       -       +       -       -
  * _REMOVE                  +       +       +       +       +
  * _REPLACE                 -       +       -       +       +
  * _SWAP                    +       +       +       +       -
  *
  * _FIRST                   +       +       +       +       +
  * _LAST                    -       -       +       +       +
  * _NEXT                    +       +       +       +       +
  * _PREV                    -       +       -       +       +
  * _EMPTY                   +       +       +       +       +
  * _FOREACH                 +       +       +       +       +
  * _FOREACH_SAFE            +       +       +       +       +
  * _FOREACH_REVERSE         -       -       -       +       +
  * _FOREACH_REVERSE_SAFE    -       -       -       +       +
  */
 
 
 /*
  * Singly-linked List definitions.
  */
 #define SLIST_HEAD(name, type)                                               \
     struct name {                                                            \
         struct type *slh_first; /* first element */                          \
     }
 
 #define SLIST_HEAD_INITIALIZER(head) { SLIST_END(head); }
 
 #define SLIST_ENTRY(type)                                                    \
     struct {                                                                 \
         struct type *sle_next; /* next element */                            \
     }
 
 /*
  * Singly-linked List functions.
  */
 #define SLIST_INIT(head) do {                                                \
     (head)->slh_first = SLIST_END(head);                                     \
 } while (0)
 
 #define SLIST_INSERT_HEAD(head, elm, field) do {                             \
     (elm)->field.sle_next = (head)->slh_first;                               \
     (head)->slh_first = (elm);                                               \
 } while (0)
 
 #define SLIST_INSERT_AFTER(slistelm, elm, field) do {                        \
     (elm)->field.sle_next = (slistelm)->field.sle_next;                      \
     (slistelm)->field.sle_next = (elm);                                      \
 } while (0)
 
 #define SLIST_REMOVE_HEAD(head, field) do {                                  \
     (head)->slh_first = (head)->slh_first->field.sle_next;                   \
 } while (0)
 
 #define SLIST_REMOVE_AFTER(slistelm, field) do {                             \
     (slistelm)->field.sle_next =                                             \
     SLIST_NEXT(SLIST_NEXT((slistelm), field), field);                        \
 } while (0)
 
 #define SLIST_REMOVE(head, elm, type, field) do {                            \
     if ((head)->slh_first == (elm)) {                                        \
         SLIST_REMOVE_HEAD((head), field);                                    \
     }                                                                        \
     else {                                                                   \
         struct type *curelm = (head)->slh_first;                             \
         while (curelm->field.sle_next != (elm)) {                            \
             curelm = curelm->field.sle_next;                                 \
         }                                                                    \
         curelm->field.sle_next = curelm->field.sle_next->field.sle_next;     \
     }                                                                        \
 } while (0)
 
 #define SLIST_SWAP(head1, head2, type) do {                                  \
     struct type *swap_first = SLIST_FIRST(head1);                            \
     SLIST_FIRST(head1) = SLIST_FIRST(head2);                                 \
     SLIST_FIRST(head2) = swap_first;                                         \
 } while (0)
 
 /*
  * Singly-linked List access methods.
  */
 #define SLIST_FIRST(head) ((head)->slh_first)
 #define SLIST_NEXT(elm, field) ((elm)->field.sle_next)
 #define SLIST_END(head) NULL
 #define SLIST_EMPTY(head) ((head)->slh_first == SLIST_END(head))
 
 #define SLIST_FOREACH(var, head, field)                                      \
     for ((var) = (head)->slh_first; (var); (var) = (var)->field.sle_next)
 
 #define SLIST_FOREACH_SAFE(var, head, field, tvar)                           \
     for ((var) = SLIST_FIRST((head));                                        \
             (var) && ((tvar) = SLIST_NEXT((var), field), 1);                 \
             (var) = (tvar))
 
 
 /*
  * List definitions.
  */
 #define LIST_HEAD(name, type)                                                \
     struct name {                                                            \
         struct type *lh_first; /* first element */                           \
     }
 
 #define LIST_HEAD_INITIALIZER(head) { LIST_END(head) }
 
 #define LIST_ENTRY(type)                                                     \
     struct {                                                                 \
         struct type *le_next; /* next element */                             \
         struct type **le_prev; /* address of previous next element */        \
     }
 
 /*
  * List functions.
  */
 #define LIST_INIT(head) do {                                                 \
     (head)->lh_first = LIST_END(head);                                       \
 } while (0)
 
 #define LIST_INSERT_HEAD(head, elm, field) do {                              \
     if (((elm)->field.le_next = (head)->lh_first) != LIST_END(head)) {       \
         (head)->lh_first->field.le_prev = &(elm)->field.le_next;             \
     }                                                                        \
     (head)->lh_first = (elm);                                                \
     (elm)->field.le_prev = &(head)->lh_first;                                \
 } while (0)
 
 #define LIST_INSERT_BEFORE(listelm, elm, field) do {                         \
     (elm)->field.le_prev = (listelm)->field.le_prev;                         \
     (elm)->field.le_next = (listelm);                                        \
     *(listelm)->field.le_prev = (elm);                                       \
     (listelm)->field.le_prev = &(elm)->field.le_next;                        \
 } while (0)
 
 #define LIST_INSERT_AFTER(listelm, elm, field) do {                          \
     if (((elm)->field.le_next = (listelm)->field.le_next) != NULL) {         \
         (listelm)->field.le_next->field.le_prev = &(elm)->field.le_next;     \
     }                                                                        \
     (listelm)->field.le_next = (elm);                                        \
     (elm)->field.le_prev = &(listelm)->field.le_next;                        \
 } while (0)
 
 #define LIST_REMOVE(elm, field) do {                                         \
     if ((elm)->field.le_next != NULL) {                                      \
         (elm)->field.le_next->field.le_prev = (elm)->field.le_prev;          \
     }                                                                        \
     *(elm)->field.le_prev = (elm)->field.le_next;                            \
 } while (0)
 
 #define LIST_REPLACE(elm, elm2, field) do {                                  \
     if (((elm2)->field.le_next = (elm)->field.le_next) != NULL) {            \
         (elm2)->field.le_next->field.le_prev = &(elm2)->field.le_next;       \
     }                                                                        \
     (elm2)->field.le_prev = (elm)->field.le_prev;                            \
     *(elm2)->field.le_prev = (elm2);                                         \
 } while (0)
 
 #define LIST_SWAP(head1, head2, type, field) do {                            \
     struct type *swap_tmp = LIST_FIRST((head1));                             \
     LIST_FIRST((head1)) = LIST_FIRST((head2));                               \
     LIST_FIRST((head2)) = swap_tmp;                                          \
     if ((swap_tmp = LIST_FIRST((head1))) != LIST_END(head1)) {               \
         swap_tmp->field.le_prev = &LIST_FIRST((head1));                      \
     }                                                                        \
     if ((swap_tmp = LIST_FIRST((head2))) != LIST_END(head2)) {               \
         swap_tmp->field.le_prev = &LIST_FIRST((head2));                      \
     }                                                                        \
 } while (0)
 
 /*
  * List access methods.
  */
 #define LIST_FIRST(head) ((head)->lh_first)
 #define LIST_NEXT(elm, field) ((elm)->field.le_next)
 #define LIST_PREV(elm, head, type, field)                                    \
     ((elm)->field.le_prev == &LIST_FIRST((head)) ? LIST_END(head) :          \
      __containerof((elm)->field.le_prev, struct type, field.le_next))
 #define LIST_END(head) NULL
 #define LIST_EMPTY(head) ((head)->lh_first == LIST_END(head))
 
 #define LIST_FOREACH(var, head, field)                                       \
     for ((var) = ((head)->lh_first);                                         \
             (var);                                                           \
             (var) = ((var)->field.le_next))
 
 #define LIST_FOREACH_SAFE(var, head, field, tvar)                            \
     for ((var) = LIST_FIRST((head));                                         \
             (var) && ((tvar) = LIST_NEXT((var), field), 1);                  \
             (var) = (tvar))
 
 
 /*
  * Singly-linked Tail Queue declarations.
  */
 #define STAILQ_HEAD(name, type)                                              \
     struct name {                                                            \
         struct type *stqh_first; /* first element */                         \
         struct type **stqh_last; /* addr of last next element */             \
     }
 
 #define STAILQ_HEAD_INITIALIZER(head) { STAILQ_END(head), &(head).stqh_first }
 
 #define STAILQ_ENTRY(type)                                                   \
     struct {                                                                 \
         struct type *stqe_next; /* next element */                           \
     }
 
 /*
  * Singly-linked Tail Queue functions.
  */
 #define STAILQ_INIT(head) do {                                               \
     (head)->stqh_first = STAILQ_END(head);                                   \
     (head)->stqh_last = &(head)->stqh_first;                                 \
 } while (0)
 
 #define STAILQ_INSERT_HEAD(head, elm, field) do {                            \
     if (((elm)->field.stqe_next = (head)->stqh_first) == STAILQ_END(head)) { \
         (head)->stqh_last = &(elm)->field.stqe_next;                         \
     }                                                                        \
     (head)->stqh_first = (elm);                                              \
 } while (0)
 
 #define STAILQ_INSERT_AFTER(head, listelm, elm, field) do {                  \
     if (((elm)->field.stqe_next = (listelm)->field.stqe_next) ==             \
             STAILQ_END(head)) {                                              \
         (head)->stqh_last = &(elm)->field.stqe_next;                         \
     }                                                                        \
     (listelm)->field.stqe_next = (elm);                                      \
 } while (0)
 
 #define STAILQ_INSERT_TAIL(head, elm, field) do {                            \
     (elm)->field.stqe_next = STAILQ_END(head);                               \
     *(head)->stqh_last = (elm);                                              \
     (head)->stqh_last = &(elm)->field.stqe_next;                             \
 } while (0)
 
 #define STAILQ_CONCAT(head1, head2) do {                                     \
     if (!STAILQ_EMPTY((head2))) {                                            \
         *(head1)->stqh_last = (head2)->stqh_first;                           \
         (head1)->stqh_last = (head2)->stqh_last;                             \
         STAILQ_INIT((head2));                                                \
     }                                                                        \
 } while (0)
 
 #define STAILQ_REMOVE_HEAD(head, field) do {                                 \
     if (((head)->stqh_first = (head)->stqh_first->field.stqe_next) ==        \
             STAILQ_END(head)) {                                              \
         (head)->stqh_last = &(head)->stqh_first;                             \
     }                                                                        \
 } while (0)
 
 #define STAILQ_REMOVE_HEAD_UNTIL(head, elm, field) do {                      \
     if ((STAILQ_FIRST((head)) = STAILQ_NEXT((elm), field)) ==                \
             STAILQ_END(head)) {                                              \
         (head)->stqh_last = &STAILQ_FIRST((head));                           \
     }                                                                        \
 } while (0)
 
 #define STAILQ_REMOVE_AFTER(head, elm, field) do {                           \
     if ((STAILQ_NEXT(elm, field) =                                           \
                 STAILQ_NEXT(STAILQ_NEXT(elm, field), field)) ==              \
             STAILQ_END(head)) {                                              \
         (head)->stqh_last = &STAILQ_NEXT((elm), field);                      \
     }                                                                        \
 } while (0)
 
 #define STAILQ_REMOVE(head, elm, type, field) do {                           \
     if ((head)->stqh_first == (elm)) {                                       \
         STAILQ_REMOVE_HEAD((head), field);                                   \
     } else {                                                                 \
         struct type *curelm = (head)->stqh_first;                            \
         while (curelm->field.stqe_next != (elm)) {                           \
             curelm = curelm->field.stqe_next;                                \
         }                                                                    \
         if ((curelm->field.stqe_next =                                       \
                     curelm->field.stqe_next->field.stqe_next) ==             \
                 STAILQ_END(head)) {                                          \
             (head)->stqh_last = &(curelm)->field.stqe_next;                  \
         }                                                                    \
     }                                                                        \
 } while (0)
 
 #define STAILQ_SWAP(head1, head2, type) do {                                 \
     struct type *swap_first = STAILQ_FIRST(head1);                           \
     struct type **swap_last = (head1)->stqh_last;                            \
     STAILQ_FIRST(head1) = STAILQ_FIRST(head2);                               \
     (head1)->stqh_last = (head2)->stqh_last;                                 \
     STAILQ_FIRST(head2) = swap_first;                                        \
     (head2)->stqh_last = swap_last;                                          \
     if (STAILQ_EMPTY(head1)) {                                               \
         (head1)->stqh_last = &STAILQ_FIRST(head1);                           \
     }                                                                        \
     if (STAILQ_EMPTY(head2)) {                                               \
         (head2)->stqh_last = &STAILQ_FIRST(head2);                           \
     }                                                                        \
 } while (0)
 
 /*
  * Singly-linked Tail Queue access methods.
  */
 #define STAILQ_FIRST(head) ((head)->stqh_first)
 #define STAILQ_LAST(head, type, field)                                       \
     (STAILQ_EMPTY((head)) ?                                                  \
      STAILQ_END(head) :                                                      \
      ((struct type *)(void *)                                                \
       ((char *)((head)->stqh_last) - offsetof(struct type, field))))
 #define STAILQ_NEXT(elm, field) ((elm)->field.stqe_next)
 #define STAILQ_END(head) NULL
 #define STAILQ_EMPTY(head) ((head)->stqh_first == STAILQ_END(head))
 
 #define STAILQ_FOREACH(var, head, field)                                     \
     for ((var) = ((head)->stqh_first);                                       \
             (var);                                                           \
             (var) = ((var)->field.stqe_next))
 
 #define STAILQ_FOREACH_SAFE(var, head, field, tvar)                          \
     for ((var) = STAILQ_FIRST((head));                                       \
             (var) && ((tvar) = STAILQ_NEXT((var), field), 1);                \
             (var) = (tvar))
 
 
 /*
  * Tail Queue definitions.
  */
 #define _TAILQ_HEAD(name, type, qual)                                        \
     struct name {                                                            \
         qual type *tqh_first; /* first element */                            \
         qual type *qual *tqh_last; /* addr of last next element */           \
     }
 #define TAILQ_HEAD(name, type) _TAILQ_HEAD(name, struct type,)
 
 #define TAILQ_HEAD_INITIALIZER(head) { TAILQ_END(head), &(head).tqh_first }
 
 #define _TAILQ_ENTRY(type, qual)                                             \
     struct {                                                                 \
         qual type *tqe_next; /* next element */                              \
         qual type *qual *tqe_prev; /* address of previous next element */    \
     }
 #define TAILQ_ENTRY(type) _TAILQ_ENTRY(struct type,)
 
 /*
  * Tail Queue functions.
  */
 #define TAILQ_INIT(head) do {                                                \
     (head)->tqh_first = TAILQ_END(head);                                     \
     (head)->tqh_last = &(head)->tqh_first;                                   \
 } while (0)
 
 #define TAILQ_INSERT_HEAD(head, elm, field) do {                             \
     if (((elm)->field.tqe_next = (head)->tqh_first) != TAILQ_END(head)) {    \
         (head)->tqh_first->field.tqe_prev = &(elm)->field.tqe_next;          \
     } else {                                                                 \
         (head)->tqh_last = &(elm)->field.tqe_next;                           \
     }                                                                        \
     (head)->tqh_first = (elm);                                               \
     (elm)->field.tqe_prev = &(head)->tqh_first;                              \
 } while (0)
 
 #define TAILQ_INSERT_BEFORE(listelm, elm, field) do {                        \
     (elm)->field.tqe_prev = (listelm)->field.tqe_prev;                       \
     (elm)->field.tqe_next = (listelm);                                       \
     *(listelm)->field.tqe_prev = (elm);                                      \
     (listelm)->field.tqe_prev = &(elm)->field.tqe_next;                      \
 } while (0)
 
 #define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {                   \
     if (((elm)->field.tqe_next = (listelm)->field.tqe_next) !=               \
             TAILQ_END(head)) {                                               \
         (elm)->field.tqe_next->field.tqe_prev = &(elm)->field.tqe_next;      \
     } else {                                                                 \
         (head)->tqh_last = &(elm)->field.tqe_next;                           \
     }                                                                        \
     (listelm)->field.tqe_next = (elm);                                       \
     (elm)->field.tqe_prev = &(listelm)->field.tqe_next;                      \
 } while (0)
 
 #define TAILQ_INSERT_TAIL(head, elm, field) do {                             \
     (elm)->field.tqe_next = TAILQ_END(head);                                 \
     (elm)->field.tqe_prev = (head)->tqh_last;                                \
     *(head)->tqh_last = (elm);                                               \
     (head)->tqh_last = &(elm)->field.tqe_next;                               \
 } while (0)
 
 #define TAILQ_CONCAT(head1, head2, field) do {                               \
     if (!TAILQ_EMPTY(head2)) {                                               \
         *(head1)->tqh_last = (head2)->tqh_first;                             \
         (head2)->tqh_first->field.tqe_prev = (head1)->tqh_last;              \
         (head1)->tqh_last = (head2)->tqh_last;                               \
         TAILQ_INIT((head2));                                                 \
     }                                                                        \
 } while (0)
 
 #define TAILQ_REMOVE(head, elm, field) do {                                  \
     if (((elm)->field.tqe_next) != TAILQ_END(head)) {                        \
         (elm)->field.tqe_next->field.tqe_prev = (elm)->field.tqe_prev;       \
     } else {                                                                 \
         (head)->tqh_last = (elm)->field.tqe_prev;                            \
     }                                                                        \
     *(elm)->field.tqe_prev = (elm)->field.tqe_next;                          \
 } while (0)
 
 #define TAILQ_REPLACE(head, elm, elm2, field) do {                           \
     if (((elm2)->field.tqe_next = (elm)->field.tqe_next) !=                  \
             TAILQ_END(head)) {                                               \
         (elm2)->field.tqe_next->field.tqe_prev = &(elm2)->field.tqe_next;    \
     } else {                                                                 \
         (head)->tqh_last = &(elm2)->field.tqe_next;                          \
     }                                                                        \
     (elm2)->field.tqe_prev = (elm)->field.tqe_prev;                          \
     *(elm2)->field.tqe_prev = (elm2);                                        \
 } while (0)
 
 #define TAILQ_SWAP(head1, head2, type, field) do {                           \
     struct type *swap_first = (head1)->tqh_first;                            \
     struct type **swap_last = (head1)->tqh_last;                             \
     (head1)->tqh_first = (head2)->tqh_first;                                 \
     (head1)->tqh_last = (head2)->tqh_last;                                   \
     (head2)->tqh_first = swap_first;                                         \
     (head2)->tqh_last = swap_last;                                           \
     if ((swap_first = (head1)->tqh_first) != TAILQ_END(head1)) {             \
         swap_first->field.tqe_prev = &(head1)->tqh_first;                    \
     } else {                                                                 \
         (head1)->tqh_last = &(head1)->tqh_first;                             \
     }                                                                        \
     if ((swap_first = (head2)->tqh_first) != TAILQ_END(head2)) {             \
         swap_first->field.tqe_prev = &(head2)->tqh_first;                    \
     } else {                                                                 \
         (head2)->tqh_last = &(head2)->tqh_first;                             \
     }                                                                        \
 } while (0)
 
 
 /*
  * Tail Queue access methods.
  */
 #define TAILQ_FIRST(head) ((head)->tqh_first)
 #define TAILQ_LAST(head, headname)                                           \
     (*(((struct headname *)((head)->tqh_last))->tqh_last))
 #define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
 #define TAILQ_PREV(elm, headname, field)                                     \
     (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))
 #define TAILQ_END(head) NULL
 #define TAILQ_EMPTY(head) ((head)->tqh_first == TAILQ_END(head))
 
 #define TAILQ_FOREACH(var, head, field)                                      \
     for ((var) = ((head)->tqh_first);                                        \
             (var) != TAILQ_END(head);                                        \
             (var) = ((var)->field.tqe_next))
 
 #define TAILQ_FOREACH_SAFE(var, head, field, next)                           \
     for ((var) = ((head)->tqh_first);                                        \
             (var) != TAILQ_END(head) &&                                      \
             ((next) = TAILQ_NEXT(var, field), 1); (var) = (next))
 
 #define TAILQ_FOREACH_REVERSE(var, head, headname, field)                    \
     for ((var) = (*(((struct headname *)((head)->tqh_last))->tqh_last));     \
             (var) != TAILQ_END(head);                                        \
             (var) = (*(((struct headname *)((var)->field.tqe_prev))->tqh_last)))
 
 #define TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, prev)         \
     for ((var) = TAILQ_LAST((head), headname);                               \
             (var) != TAILQ_END(head) &&                                      \
             ((prev) = TAILQ_PREV((var), headname, field), 1); (var) = (prev))
 
 
 /*
  * Circular Queue definitions.
  */
 #define CIRCLEQ_HEAD(name, type)                                             \
     struct name {                                                            \
         struct type *cqh_first; /* first element */                          \
         struct type *cqh_last; /* last element */                            \
     }
 
 #define CIRCLEQ_HEAD_INITIALIZER(head) { (void *)&head, (void *)&head }
 
 #define CIRCLEQ_ENTRY(type)                                                  \
     struct {                                                                 \
         struct type *cqe_next; /* next element */                            \
         struct type *cqe_prev; /* previous element */                        \
     }
 
 /*
  * Circular Queue functions.
  */
 #define CIRCLEQ_INIT(head) do {                                              \
     (head)->cqh_first = (void *)(head);                                      \
     (head)->cqh_last = (void *)(head);                                       \
 } while (0)
 
 #define CIRCLEQ_INSERT_HEAD(head, elm, field) do {                           \
     (elm)->field.cqe_next = (head)->cqh_first;                               \
     (elm)->field.cqe_prev = (void *)(head);                                  \
     if ((head)->cqh_last == (void *)(head)) {                                \
         (head)->cqh_last = (elm);                                            \
     } else {                                                                 \
         (head)->cqh_first->field.cqe_prev = (elm);                           \
     }                                                                        \
     (head)->cqh_first = (elm);                                               \
 } while (0)
 
 #define CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field) do {                \
     (elm)->field.cqe_next = (listelm);                                       \
     (elm)->field.cqe_prev = (listelm)->field.cqe_prev;                       \
     if ((listelm)->field.cqe_prev == (void *)(head)) {                       \
         (head)->cqh_first = (elm);                                           \
     } else {                                                                 \
         (listelm)->field.cqe_prev->field.cqe_next = (elm);                   \
     }                                                                        \
     (listelm)->field.cqe_prev = (elm);                                       \
 } while (0)
 
 #define CIRCLEQ_INSERT_AFTER(head, listelm, elm, field) do {                 \
     (elm)->field.cqe_next = (listelm)->field.cqe_next;                       \
     (elm)->field.cqe_prev = (listelm);                                       \
     if ((listelm)->field.cqe_next == (void *)(head)) {                       \
         (head)->cqh_last = (elm);                                            \
     } else {                                                                 \
         (listelm)->field.cqe_next->field.cqe_prev = (elm);                   \
     }                                                                        \
     (listelm)->field.cqe_next = (elm);                                       \
 } while (0)
 
 #define CIRCLEQ_INSERT_TAIL(head, elm, field) do {                           \
     (elm)->field.cqe_next = (void *)(head);                                  \
     (elm)->field.cqe_prev = (head)->cqh_last;                                \
     if ((head)->cqh_first == (void *)(head)) {                               \
         (head)->cqh_first = (elm);                                           \
     } else {                                                                 \
         (head)->cqh_last->field.cqe_next = (elm);                            \
     }                                                                        \
     (head)->cqh_last = (elm);                                                \
 } while (0)
 
 #define CIRCLEQ_REMOVE(head, elm, field) do {                                \
     if ((elm)->field.cqe_next == (void *)(head)) {                           \
         (head)->cqh_last = (elm)->field.cqe_prev;                            \
     } else {                                                                 \
         (elm)->field.cqe_next->field.cqe_prev = (elm)->field.cqe_prev;       \
     }                                                                        \
     if ((elm)->field.cqe_prev == (void *)(head)) {                           \
         (head)->cqh_first = (elm)->field.cqe_next;                           \
     } else {                                                                 \
         (elm)->field.cqe_prev->field.cqe_next = (elm)->field.cqe_next;       \
     }                                                                        \
 } while (0)
 
 #define CIRCLEQ_REPLACE(head, elm, elm2, field) do {                         \
     if (((elm2)->field.cqe_next = (elm)->field.cqe_next) ==                  \
             CIRCLEQ_END(head)) {                                             \
         (head).cqh_last = (elm2);                                            \
     } else {                                                                 \
         (elm2)->field.cqe_next->field.cqe_prev = (elm2);                     \
     }                                                                        \
     if (((elm2)->field.cqe_prev = (elm)->field.cqe_prev) ==                  \
             CIRCLEQ_END(head)) {                                             \
         (head).cqh_first = (elm2);                                           \
     } else {                                                                 \
         (elm2)->field.cqe_prev->field.cqe_next = (elm2);                     \
     }                                                                        \
 } while (0)
 
 /*
  * Circular Queue access methods.
  */
 #define CIRCLEQ_FIRST(head) ((head)->cqh_first)
 #define CIRCLEQ_LAST(head) ((head)->cqh_last)
 #define CIRCLEQ_NEXT(elm, field) ((elm)->field.cqe_next)
 #define CIRCLEQ_PREV(elm, field) ((elm)->field.cqe_prev)
 #define CIRCLEQ_END(head) ((void *)(head))
 #define CIRCLEQ_EMPTY(head) (CIRCLEQ_FIRST(head) == CIRCLEQ_END(head))
 
 #define CIRCLEQ_FOREACH(var, head, field)                                    \
     for ((var) = CIRCLEQ_FIRST(head);                                        \
             (var) != CIRCLEQ_END(head);                                      \
             (var) = CIRCLEQ_NEXT(var, field))
 
 #define CIRCLEQ_FOREACH_SAFE(var, head, field, tvar)                         \
     for ((var) = CIRCLEQ_FIRST(head);                                        \
             (var) != CIRCLEQ_END(head) &&                                    \
             ((tvar) = CIRCLEQ_NEXT(var, field), 1);                          \
             (var) = (tvar))
 
 #define CIRCLEQ_FOREACH_REVERSE(var, head, field)                            \
     for ((var) = CIRCLEQ_LAST(head);                                         \
             (var) != CIRCLEQ_END(head);                                      \
             (var) = CIRCLEQ_PREV(var, field))
 
 #define CIRCLEQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)       \
     for ((var) = CIRCLEQ_LAST(head, headname);                               \
             (var) != CIRCLEQ_END(head) &&                                    \
             ((tvar) = CIRCLEQ_PREV(var, headname, field), 1);                \
             (var) = (tvar))
 
 #define CIRCLEQ_LOOP_NEXT(head, elm, field)                                  \
     (((elm)->field.cqe_next == (void *)(head))                               \
      ? ((head)->cqh_first)                                                   \
      : (elm->field.cqe_next))
 
 #define CIRCLEQ_LOOP_PREV(head, elm, field)                                  \
     (((elm)->field.cqe_prev == (void *)(head))                               \
      ? ((head)->cqh_last)                                                    \
      : (elm->field.cqe_prev))
 
 #endif /* !_SYS_QUEUE_H_ */