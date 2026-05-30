#include <stdio.h>
#include <stdlib.h>

#define RED   0
#define BLACK 1

typedef struct TreeNode
{
    int data;
    int color;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
} TreeNode;

typedef struct set
{
    TreeNode *root;
    TreeNode *NIL;
} set;

static TreeNode *createNIL()
{
    TreeNode *nil = (TreeNode *)malloc(sizeof(TreeNode));
    nil->data   = 0;
    nil->color  = BLACK;
    nil->left   = NULL;
    nil->right  = NULL;
    nil->parent = NULL;
    return nil;
}

void intialize(set *s)
{
    s->NIL        = createNIL();
    s->root       = s->NIL;
}

static void rotateLeft(set *s, TreeNode *x)
{
    TreeNode *y = x->right;
    x->right    = y->left;

    if (y->left != s->NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == s->NIL)
        s->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left   = x;
    x->parent = y;
}

static void rotateRight(set *s, TreeNode *x)
{
    TreeNode *y = x->left;
    x->left     = y->right;

    if (y->right != s->NIL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == s->NIL)
        s->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right  = x;
    x->parent = y;
}

static void insertFixup(set *s, TreeNode *z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {

            TreeNode *uncle = z->parent->parent->right;

            if (uncle->color == RED)
            {

                z->parent->color          = BLACK;
                uncle->color              = BLACK;
                z->parent->parent->color  = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {

                    z = z->parent;
                    rotateLeft(s, z);
                }

                z->parent->color         = BLACK;
                z->parent->parent->color = RED;
                rotateRight(s, z->parent->parent);
            }
        }
        else
        {

            TreeNode *uncle = z->parent->parent->left;

            if (uncle->color == RED)
            {

                z->parent->color         = BLACK;
                uncle->color             = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {

                    z = z->parent;
                    rotateRight(s, z);
                }

                z->parent->color         = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(s, z->parent->parent);
            }
        }
    }
    s->root->color = BLACK;
}

void insert(set *s, int val)
{

    TreeNode *cur = s->root;
    while (cur != s->NIL)
    {
        if (val == cur->data) return;
        cur = (val < cur->data) ? cur->left : cur->right;
    }

    TreeNode *z  = (TreeNode *)malloc(sizeof(TreeNode));
    z->data      = val;
    z->color     = RED;
    z->left      = s->NIL;
    z->right     = s->NIL;
    z->parent    = s->NIL;

    TreeNode *parent = s->NIL;
    TreeNode *node   = s->root;
    while (node != s->NIL)
    {
        parent = node;
        node   = (val < node->data) ? node->left : node->right;
    }

    z->parent = parent;

    if (parent == s->NIL)
        s->root = z;
    else if (val < parent->data)
        parent->left  = z;
    else
        parent->right = z;

    insertFixup(s, z);
}

int search(set s, int val)
{
    TreeNode *cur = s.root;
    while (cur != s.NIL)
    {
        if (cur->data == val)  return 1;
        cur = (val < cur->data) ? cur->left : cur->right;
    }
    return 0;
}

static void rbTransplant(set *s, TreeNode *u, TreeNode *v)
{
    if (u->parent == s->NIL)
        s->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

static TreeNode *treeMinimum(set *s, TreeNode *node)
{
    while (node->left != s->NIL)
        node = node->left;
    return node;
}

static void deleteFixup(set *s, TreeNode *x)
{
    while (x != s->root && x->color == BLACK)
    {
        if (x == x->parent->left)
        {
            TreeNode *w = x->parent->right;

            if (w->color == RED)
            {

                w->color           = BLACK;
                x->parent->color   = RED;
                rotateLeft(s, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK)
            {

                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {

                    w->left->color = BLACK;
                    w->color       = RED;
                    rotateRight(s, w);
                    w = x->parent->right;
                }

                w->color         = x->parent->color;
                x->parent->color = BLACK;
                w->right->color  = BLACK;
                rotateLeft(s, x->parent);
                x = s->root;
            }
        }
        else
        {

            TreeNode *w = x->parent->left;

            if (w->color == RED)
            {

                w->color         = BLACK;
                x->parent->color = RED;
                rotateRight(s, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK)
            {

                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {

                    w->right->color = BLACK;
                    w->color        = RED;
                    rotateLeft(s, w);
                    w = x->parent->left;
                }

                w->color         = x->parent->color;
                x->parent->color = BLACK;
                w->left->color   = BLACK;
                rotateRight(s, x->parent);
                x = s->root;
            }
        }
    }
    x->color = BLACK;
}

void deleteset(set *s, int key)
{

    TreeNode *z = s->root;
    while (z != s->NIL)
    {
        if (z->data == key)  break;
        z = (key < z->data) ? z->left : z->right;
    }
    if (z == s->NIL) return;

    TreeNode *y              = z;
    int       yOriginalColor = y->color;
    TreeNode *x;

    if (z->left == s->NIL)
    {

        x = z->right;
        rbTransplant(s, z, z->right);
    }
    else if (z->right == s->NIL)
    {

        x = z->left;
        rbTransplant(s, z, z->left);
    }
    else
    {

        y             = treeMinimum(s, z->right);
        yOriginalColor = y->color;
        x              = y->right;

        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            rbTransplant(s, y, y->right);
            y->right         = z->right;
            y->right->parent = y;
        }

        rbTransplant(s, z, y);
        y->left         = z->left;
        y->left->parent = y;
        y->color        = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK)
        deleteFixup(s, x);
}

static TreeNode *copyTreeHelper(TreeNode *node, TreeNode *srcNIL,
                                TreeNode *dstNIL, TreeNode *parent)
{
    if (node == srcNIL)
        return dstNIL;

    TreeNode *newnode  = (TreeNode *)malloc(sizeof(TreeNode));
    newnode->data      = node->data;
    newnode->color     = node->color;
    newnode->parent    = parent;
    newnode->left      = copyTreeHelper(node->left,  srcNIL, dstNIL, newnode);
    newnode->right     = copyTreeHelper(node->right, srcNIL, dstNIL, newnode);
    return newnode;
}

static set copySet(set src)
{
    set dst;
    intialize(&dst);
    dst.root = copyTreeHelper(src.root, src.NIL, dst.NIL, dst.NIL);
    return dst;
}

void mergeset(set *st1, TreeNode *st2, TreeNode *srcNIL)
{
    if (st2 == srcNIL) return;
    insert(st1, st2->data);
    mergeset(st1, st2->left,  srcNIL);
    mergeset(st1, st2->right, srcNIL);
}

set unionset(set st1, set st2)
{
    if (st1.root == st1.NIL) return st2;
    if (st2.root == st2.NIL) return st1;

    set newset = copySet(st1);
    mergeset(&newset, st2.root, st2.NIL);
    return newset;
}

void intersection(set *st3, TreeNode *st1, TreeNode *st1NIL, set st2)
{
    if (st1 == st1NIL) return;
    if (search(st2, st1->data))
        insert(st3, st1->data);
    intersection(st3, st1->left,  st1NIL, st2);
    intersection(st3, st1->right, st1NIL, st2);
}

set intersectset(set st1, set st2)
{
    set st3;
    intialize(&st3);
    if (st1.root == st1.NIL || st2.root == st2.NIL) return st3;
    intersection(&st3, st1.root, st1.NIL, st2);
    return st3;
}

void removeelement(set *st4, TreeNode *root, TreeNode *srcNIL)
{
    if (root == srcNIL) return;
    deleteset(st4, root->data);
    removeelement(st4, root->left,  srcNIL);
    removeelement(st4, root->right, srcNIL);
}

set differenceset(set st1, set st2)
{
    if (st2.root == st2.NIL || st1.root == st1.NIL) return st1;
    set temp = intersectset(st1, st2);
    set st4  = copySet(st1);
    removeelement(&st4, temp.root, temp.NIL);
    return st4;
}

void Innode(TreeNode *temp, TreeNode *NIL)
{
    if (temp == NIL) return;
    Innode(temp->left, NIL);
    printf("%d ", temp->data);
    Innode(temp->right, NIL);
}

void display(set st)
{
    Innode(st.root, st.NIL);
}

int main()
{
    set st1, st2;
    intialize(&st1);
    intialize(&st2);

    int n;
    printf("Enter the Number of elements to insert in set1 ");
    scanf("%d", &n);
    printf("Enter the elements to insert ");
    for (int i = 0; i < n; i++)
    {
        int x;
        scanf("%d", &x);
        insert(&st1, x);
    }

    int m;
    printf("Enter the Number of elements to insert in set2 ");
    scanf("%d", &m);
    printf("Enter the elements to insert ");
    for (int i = 0; i < m; i++)
    {
        int x;
        scanf("%d", &x);
        insert(&st2, x);
    }

    set st3 = unionset(st1, st2);
    printf("Union :");
    display(st3);
    printf("\n");

    st3 = intersectset(st1, st2);
    printf("Intersection :");
    display(st3);
    printf("\n");

    st3 = differenceset(st1, st2);
    printf("Difference :");
    display(st3);
    printf("\n");

    return 0;
}