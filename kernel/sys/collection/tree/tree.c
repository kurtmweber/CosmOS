
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/tree/tree.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/assert.h>

struct tree* tree_new() {
    struct tree* ret = (struct tree*) kmalloc(sizeof(struct tree));
    ret->key=0;
    ret->value=0;
    ret->left=0;
    ret->right=0;
    return ret; 
}

void tree_delete(struct tree* t) {
    ASSERT_NOT_NULL(t, "t must not be null");
    if (0!=t->left){
        tree_delete(t->left);
    }
    if (0!=t->right){
        tree_delete(t->right);
    }
    kfree(t);
}

void tree_insert(struct tree* t, uint32_t key, void* value) {
    ASSERT_NOT_NULL(t, "t must not be null");
    if (key < t->key){
        if (0!=t->left){
            tree_insert(t->left, key,value);
        } else {
            t->left = tree_new();
            t->left->key=key;
            t->left->value=value;
        }
    } else {
        if (0!=t->right){
            tree_insert(t->right, key,value);
        }else {
            t->right = tree_new();
            t->right->key=key;
            t->right->value=value;
        }
    }
}

void* tree_search(struct tree* t, uint32_t key) {
    ASSERT_NOT_NULL(t, "t must not be null");
    if (key==t->key){
        return t->value;
    } else if (key <t->key){
        if (0!=t->left){
            return tree_search(t->left, key);
        } else {
            return 0;
        }
    } else {
        if (0!=t->right){
            return tree_search(t->right, key);
        } else {
            return 0;
        } 
    }
}