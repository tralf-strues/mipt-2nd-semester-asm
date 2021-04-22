#pragma once

#include <stdarg.h>
#include "syntax.h"

union NodeData
{
    int64_t     number;
    MathOp      operation;
    const char* id;
    bool        isVoidFunction;
};

// FIXME: change all names 
enum NodeType
{
    FDECL_TYPE,
    VDECL_TYPE,
    // ADECL_TYPE,      TODO:
    // MEM_ACCESS_TYPE, TODO:
    ID_TYPE,
    EXPR_LIST_TYPE,

    BLOCK_TYPE,
    STATEMENT_TYPE,

    COND_TYPE,
    IFELSE_TYPE,
    LOOP_TYPE,
    ASSIGN_TYPE,

    CALL_TYPE,
    JUMP_TYPE,

    MATH_TYPE,
    NUMBER_TYPE,

    TYPES_COUNT
};

struct Node
{
    NodeType type;
    NodeData data;

    Node*    parent;
    Node*    left;
    Node*    right;
};

#define BINARY_OP(op, root1, root2) newNode(MATH_TYPE, { .operation = op##_OP  }, root1,   root2)
#define ID(idString)                newNode(ID_TYPE,   { .id        = idString }, nullptr, nullptr)

void   destroySubtree    (Node* root);

Node*  newNode           ();
Node*  newNode           (NodeType type, NodeData data, Node* left, Node* right);
void   deleteNode        (Node* node);

void   setLeft           (Node* root, Node* left);
void   setRight          (Node* root, Node* right);

void   copyNode          (Node* dest, const Node* src);
Node*  copyTree          (const Node* root);

bool   isLeft            (const Node* node);

void   setData           (Node* node, NodeType type, NodeData data);
void   setData           (Node* node, int64_t number);
void   setData           (Node* node, MathOp op);
void   setData           (Node* node, const char* id);
void   setData           (Node* node, bool isVoidFunction);

int    counterFileUpdate (const char* filename);
void   graphSimpleDump   (const Node* root, const char* treeFilename, const char* outputFilename);
void   graphDetailedDump (const Node* root, const char* treeFilename, const char* outputFilename);

void   dumpToFile        (FILE* file, const Node* root);
Node*  readTreeFromFile  (const char* filename);

const char* nodeTypeToString(NodeType type);