#include <any>
typedef struct bin_tree {
    std::any value;        
    struct bin_tree *left;
    struct bin_tree *right;
    int depth;
} BinaryTree_t;

enum class SIDE {
   LEFT,
   RIGHT
};

class BinaryTree {
private:
   BinaryTree_t *current_bin_tree;
   BinaryTree_t *root_bin_tree;
public:
   BinaryTree(int root_value);
   // mirrors right and left side of the tree
   void mirror();
   int depth();
   BinaryTree_t *get_root();
   BinaryTree_t *get_left();
   BinaryTree_t *get_right();
   void set_value();
   void get_value();
   // switches current branch with defined side branch
   void switch_branch(SIDE);
};
