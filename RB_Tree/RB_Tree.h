#ifndef RB_TREE_H
#define RB_TREE_H

#include "RB_Tree_Node.h"
#include <iostream>
#include <queue>

template<class T>
class RB_Tree{
public:
    RB_Tree(T Root_data);
    ~RB_Tree();
    void Left_Rotate(RB_Tree_Node<T> * current_node);
    void Right_Rotate(RB_Tree_Node<T>* current_node);
	void Insert_Node(T insert_data);
	int Delete_Node(T delete_data);
	void Fix_Tree(RB_Tree_Node<T>* current_node);
	void erase_Node(RB_Tree_Node<T>* Node_del);
	void Fix_Tree_Delete(RB_Tree_Node<T>* current_node);
	RB_Tree_Node<T>* Find_Successor_Node(RB_Tree_Node<T>* current_node);
	void display();
	RB_Tree_Node<T>* Root_Node;
};

template <class T>
RB_Tree<T>::RB_Tree(T Root_Data):Root_Node(nullptr){
    if(Root_Node == nullptr){
        Root_Node = new RB_Tree_Node<T>(Root_Data);
        Root_Node->color_tag = 0;
    }
}

template <class T>
RB_Tree<T>::~RB_Tree(){

}

template <class T>
void RB_Tree<T>::display(){
    RB_Tree_Node<T> *temp_node = Root_Node;
    std::queue<RB_Tree_Node<T>*> q;
	q.push(temp_node);
    while(!q.empty()){
		temp_node = q.front();
        std::cout<<" "<<temp_node->data<<std::endl;
		q.pop();
        if(temp_node->left_child!=nullptr){
            q.push(temp_node->left_child);
            continue;
        }
        if(temp_node->right_child!=nullptr){
            q.push(temp_node->right_child);
            continue;
        }
    }
}
template <class T>
void RB_Tree<T>::Insert_Node(T insert_data){
    RB_Tree_Node<T> *temp_node = Root_Node;
    while(temp_node != nullptr){
        if(insert_data > temp_node->data){
            if(temp_node->right_child == nullptr){
                temp_node->right_child = new RB_Tree_Node<T>(insert_data);
                temp_node->right_child->color_tag = 1;
                temp_node->father_node = temp_node;
                if(temp_node->color_tag == 1){
                    Fix_Tree(temp_node->right_child);
                }
                break;
            }else {
                temp_node = temp_node->right_child;
            }
        }else{
			if (temp_node->left_child == NULL){
				temp_node->left_child  = new RB_Tree_Node<T>(insert_data);
				temp_node->left_child->color_tag = 1;
				temp_node->left_child->father_node = temp_node;
				if (temp_node->color_tag == 1){
					Fix_Tree(temp_node->left_child);
				}				
				break;
			}else{
				temp_node = temp_node->left_child;
			}			
		}
    }
}

template<class T>
int RB_Tree<T>::Delete_Node(T delete_data)
{	
	RB_Tree_Node<T>* temp_Node = Root_Node;
	while(temp_Node->data != delete_data && temp_Node != NULL)
	{
		if (delete_data > temp_Node->data)
		{
			temp_Node = temp_Node->right_child;
		}
		else
		{
			temp_Node = temp_Node->left_child;
		}
	}	
	if (temp_Node)
	//?????????????????????
	{
		int color_tag = temp_Node->color_tag;
		
		if (temp_Node->left_child == NULL && temp_Node->right_child == NULL)
		//?????????????????????????????????
		{
			//			
			delete temp_Node;
		}
		else
		if (temp_Node->left_child == NULL && temp_Node->right_child != NULL)
		//???????????????,??????????????????
		{				
			if (temp_Node != Root_Node)
			//???????????????
			{
				if (temp_Node->Father_Node->left_child == temp_Node)
				{
					temp_Node->Father_Node->left_child = temp_Node->right_child;
					temp_Node->right_child->Father_Node = temp_Node->Father_Node;
					
				}
				else
				{
					temp_Node->Father_Node->right_child = temp_Node->right_child;
					temp_Node->right_child->Father_Node = temp_Node->Father_Node;
					
				}
			}
			else
			//?????????
			{
				Root_Node = temp_Node->right_child;
				temp_Node->right_child->Father_Node = Root_Node;
				
			}
		}
		else if (temp_Node->left_child != NULL && temp_Node->right_child == NULL)
		//??????????????????,???????????????
		{
			if (temp_Node != Root_Node)
				//???????????????
			{
				if (temp_Node->Father_Node->left_child == temp_Node)
				{
					temp_Node->Father_Node->left_child = temp_Node->left_child;
					temp_Node->left_child->Father_Node = temp_Node->Father_Node;
					
				}
				else
				{
					temp_Node->Father_Node->right_child = temp_Node->left_child;
					temp_Node->left_child->Father_Node = temp_Node->Father_Node;
					
				}
			}
			else
				//?????????
			{
				Root_Node = temp_Node->left_child;
				temp_Node->left_child->Father_Node = Root_Node;
				//delete temp_Node;
			}
		}
		else if (temp_Node->left_child != NULL && temp_Node->right_child != NULL)
		//????????????????????????
		{
			RB_Tree_Node<T>* Successor_Node = Find_Successor_Node(temp_Node);
			if (temp_Node == temp_Node->Father_Node->left_child)
			{
				temp_Node->Father_Node->left_child = Successor_Node;
				Successor_Node->right_child = temp_Node->right_child;
				Successor_Node->left_child = temp_Node->left_child;
				Successor_Node->Father_Node = temp_Node->Father_Node;
				Successor_Node->color_tag = temp_Node->color_tag;
				if (Successor_Node->right_child)
				{
					Successor_Node->Father_Node->left_child = Successor_Node->right_child;
					Successor_Node->right_child->Father_Node = Successor_Node->Father_Node;
				}
				
			}
			else
			{
				temp_Node->Father_Node->right_child = Successor_Node;
				Successor_Node->right_child = temp_Node->right_child;
				Successor_Node->left_child = temp_Node->left_child;
				Successor_Node->Father_Node = temp_Node->Father_Node;
				Successor_Node->color_tag = temp_Node->color_tag;
				if (Successor_Node->right_child)
				{
					Successor_Node->Father_Node->left_child = Successor_Node->right_child;
					Successor_Node->right_child->Father_Node = Successor_Node->Father_Node;
				}
			}			
		}
		
		if (color_tag == 0)
		//??????????????????????????????  ?????????????????????
		{
			Fix_Tree_Delete(temp_Node);
		}
	}
	//????????????????????????  ??????-1
	else
	{
		return -1;
	}
	return 1;
	
}

template <class T>
void RB_Tree<T>::Fix_Tree(RB_Tree_Node<T>* current_Node)
{
	RB_Tree_Node<T>* temp_current_Node = current_Node;	
	RB_Tree_Node<T>* uncle_Node;	
		
	while(true)
	{
		if ( temp_current_Node->father_node == NULL)
		{
			break;
		}
		if (temp_current_Node->father_node->color_tag != 1)
		{
			break;
		}		 
		RB_Tree_Node<T>* father_node = temp_current_Node->father_node;
		RB_Tree_Node<T>* grandfa_Node = father_node->father_node;
		if (grandfa_Node)
		{
			if (father_node == grandfa_Node->left_child)
			{
				uncle_Node = grandfa_Node->right_child;
				//????????????????????????
				if (uncle_Node)
				{
					//??????1 ???????????????  ????????????????????????????????????????????? 
					//??????????????????????????? ????????????????????????????????????
					if (uncle_Node->color_tag == 1)
					{
						uncle_Node->color_tag = 0;
						father_node->color_tag = 0;						
						grandfa_Node->color_tag = 1;						
						temp_current_Node = grandfa_Node;						
					}
					//??????2?????????????????? ??????????????????????????? ?????????????????????????????? ????????????????????????
					else if (temp_current_Node == father_node->right_child)
					{
						temp_current_Node = temp_current_Node->father_node;						
						Left_Rotate(temp_current_Node);					
					}
					//??????3?????????????????? ??????????????????????????? ???????????????????????? ???????????????????????? ?????????????????????
					else
					{
						father_node->color_tag = 0;
						grandfa_Node->color_tag = 1;
						Right_Rotate(grandfa_Node);				
					}					
				}
				//?????????????????????
				else
				{
					if (temp_current_Node == father_node->right_child)
					{
						temp_current_Node = temp_current_Node->father_node;						
						Left_Rotate(temp_current_Node);					
					}					
					else
					{
						father_node->color_tag = 0;
						grandfa_Node->color_tag = 1;
						Right_Rotate(grandfa_Node);	
					}
				}
			}
			else
			{
				uncle_Node = grandfa_Node->left_child;
				if (uncle_Node)
				{
					//??????1 ???????????????  ????????????????????????????????????????????? 
					//??????????????????????????? ????????????????????????????????????
					if (uncle_Node->color_tag == 1)
					{
						uncle_Node->color_tag = 0;
						father_node->color_tag = 0;						
						grandfa_Node->color_tag = 1;						
						temp_current_Node = grandfa_Node;						
					}
					//??????2?????????????????? ??????????????????????????? ?????????????????????????????? ????????????????????????
					else if (temp_current_Node == father_node->left_child)
					{
						temp_current_Node = temp_current_Node->father_node;
						Right_Rotate(temp_current_Node);						
					}
					//??????3?????????????????? ??????????????????????????? ???????????????????????? ???????????????????????? ?????????????????????
					else
					{
						father_node->color_tag = 0;
						grandfa_Node->color_tag = 1;
						Left_Rotate(grandfa_Node);	
					}									
				}
				else
				{
					if (temp_current_Node == father_node->left_child)
					{
						temp_current_Node = temp_current_Node->father_node;
						Right_Rotate(temp_current_Node);						
					}					
					else
					{
						father_node->color_tag = 0;
						grandfa_Node->color_tag = 1;
						Left_Rotate(grandfa_Node);	
					}	
				}
			}
		}
	}
	Root_Node->color_tag = 0;

}

template <class T>
void RB_Tree<T>::Left_Rotate(RB_Tree_Node<T> * current_node)
{
	RB_Tree_Node<T>* Right_child = current_node->right_child;	
	RB_Tree_Node<T>* Father_node = current_node->father_node;
	current_node->right_child = Right_child->left_child;
	Right_child->father_node = Father_node;
	if (Father_node == NULL){
		Root_Node = Right_child;
	}else if(current_node == Father_node->left_child){
		Father_node->left_child = Right_child;
	}else{
		Father_node->right_child = Right_child;
	}	

	Right_child->left_child = current_node;
	current_node->father_node = Right_child;

}

template <class T>
void RB_Tree<T>::Right_Rotate(RB_Tree_Node<T>* current_node)
{
	RB_Tree_Node<T>* left_Node = current_node->left_child;
	RB_Tree_Node<T>* Father_node = current_node->father_node;
	current_node->left_child = left_Node->right_child;
	left_Node->right_child = current_node;
	if (Father_node == NULL)
	{
		Root_Node = left_Node;
	}
	else if (current_node = Father_node->left_child)
	{
		Father_node->left_child = left_Node;
	}
	else
	{
		Father_node->right_child = left_Node;
	}
	current_node->father_node = left_Node;
	left_Node->father_node = Father_node;
}

template<class T>
void RB_Tree<T>::erase_Node(RB_Tree_Node<T>* Node_del)
{
	if (Node_del->father_node)
	{
		Node_del->father_node = NULL;
	}
	Node_del->color_tag = NULL;
	Node_del->father_node = NULL;
	Node_del->left_child = NULL;
	Node_del->Right_child = NULL;
	Node_del->data = NULL;	
	delete Node_del;
}
#endif // RB_TREE_H