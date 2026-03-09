#ifndef RB_TREE_NODE_H
#define RB_TREE_NODE_H

template <class T>
class RB_Tree_Node
{
public:
    RB_Tree_Node(T data_in);
    ~RB_Tree_Node(void);
    RB_Tree_Node *left_child;
    RB_Tree_Node *right_child;
    RB_Tree_Node *father_node;
    T data;
    int color_tag; // 1 Red 0 Black
};
template <typename T>
RB_Tree_Node<T>::RB_Tree_Node(T data_in) : data(data_in), color_tag(0),
                                           left_child(nullptr), right_child(nullptr), father_node(nullptr)
{
    data = data_in;
    color_tag = 1;
}
template <typename T>
RB_Tree_Node<T>::~RB_Tree_Node(void)
{
}
#endif // RB_TREE_NODE_H