/*
 * 树的操作.树元素不重复
 * */
#include<stdio.h>
#include<stdlib.h>
#define N 10000			//插入节点个数i 2^13
#define LL 3
#define LR 1
#define RR -3
#define RL -1
typedef struct tree {
	int data;
	signed int bf;		//平衡因子
	struct tree *lift;
	struct tree *right;
} Tree;
Tree *create_leaf(int value)
{
	Tree *newleaf = (Tree *) malloc(sizeof(Tree));
	newleaf->bf = 0;
	newleaf->data = value;
	newleaf->lift = NULL;
	newleaf->right = NULL;
	return newleaf;

}

void preorder(Tree * root)	//先序
{
	if (root == NULL)
		return;
	printf("%d ", root->data);	//遍历中的操作,这里仅仅打印.
	preorder(root->lift);
	preorder(root->right);
}

void inorder(Tree * root)	//中序
{
	if (root == NULL)
		return;
	inorder(root->lift);
	printf("%d ", root->data);
	inorder(root->right);
}

void postorder(Tree * root)	//后序
{
	if (root == NULL) {
		return;
	}
	postorder(root->lift);
	postorder(root->right);
	printf("%d ", root->data);
}

//向树中插入叶,中序插入,有相同值不操作.
void insert_tree(Tree * root, int value)	//插入一个已经存在的节点,不操作,插入成中序.
{
	if (value < root->data) {	//比根小,插入成左叶子.
		if (root->lift == NULL) {	//空,插入
			root->lift = create_leaf(value);
			return;
		} else {	//非空,递归
			insert_tree(root->lift, value);
		}
	} else if (value > root->data) {	//比跟大,插入成右叶子
		if (root->right == NULL) {	//空,插入
			root->right = create_leaf(value);
			return;
		} else {	//非空,递归
			insert_tree(root->right, value);
		}
	} else {		//与根相等,不操作
		return;
	}
}

int length(Tree * root)		// 高度
{
	if (root == NULL)
		return 0;
	else {
		int i = 0, j = 0;
		i = 1 + length(root->lift);
		j = 1 + length(root->right);
		return i > j ? i : j;
	}
}

Tree *del_node(Tree * p)
{
	if (p->lift != NULL) {
		Tree *r = p->lift;	//r指向其左子树;
		while (r->right != NULL)	//搜索左子树的最右边的叶子结点r
		{
			r = r->right;
		}
		r->right = p->right;

		Tree *q = p->lift;	//q指向其左子树;
		free(p);
		return q;
	} else {
		Tree *q = p->right;	//q指向其右子树;
		free(p);
		return q;
	}
}

Tree *del_tree(Tree * root, int value)
{
	if (root != NULL) {
		if (root->data == value) {
			root = del_node(root);
		} else if (root->data > value) {	//没找到,递归查找
			root->lift = del_tree(root->lift, value);

		} else {
			root->right = del_tree(root->right, value);
		}
	}
	return root;
}

// 按值 沿顶到底查找并更新平衡因子.
/*
 * Tree *updatebf(Tree * root, int value)
{
	if (root != NULL) {
		if (root->data == value) {
			//root = del_node(root);
			//计算插入值的平衡因子
			root->bf = length(root->lift) - length(root->right);
		} else if (root->data > value) {	//没找到,递归查找

			root->bf = length(root->lift) - length(root->right);
			root->lift = updatebf(root->lift, value);

		} else {
			root->bf = length(root->lift) - length(root->right);
			root->right = updatebf(root->right, value);
		}
	}
	return root;
}
*/
//更新平衡因子并且返回找到的a值
Tree *finda(Tree * root, int value)	//循环查找节点
{
	Tree *a = NULL;
	while (root != NULL) {
		if (root->data == value) {
			root->bf = length(root->lift) - length(root->right);
			if (root->bf == 2 || root->bf == -2) {
				a = root;
			} else {
				return a;
			}
		}
		if (root->data > value) {
			root->bf = length(root->lift) - length(root->right);

			if (root->lift != NULL) {
				if (root->bf == 2 || root->bf == -2) {
					a = root;
				}
				root = root->lift;
			}
		}
		if (root->data < value) {
			root->bf = length(root->lift) - length(root->right);

			if (root->right != NULL) {
				if (root->bf == 2 || root->bf == -2) {
					a = root;
				}
				root = root->right;
			}
		}

	}
	return a;
}

//判断a的类型 LL LR RR RL四种
int adjust(Tree * a)
{
	if (a->bf == 2 && a->lift != NULL) {
		return a->lift->bf + a->bf;
	}
	if (a->bf == -2 && a->right != NULL) {
		return a->right->bf + a->bf;
	}
	printf("err in adjust type of a\n");
	return 0;
}

Tree *findparent(Tree * root, Tree * child)	//循环查找父节点
{
	Tree *p=NULL;
	if (root == child)
		return p;
	while (root != NULL) {
		if (root == child) {
			return p;
		}
		p = root;
		if (root->data > child->data) {
			if (root->lift != NULL) {
				root = root->lift;
				continue;
			}
		}
		if (root->data < child->data) {
			if (root->right != NULL) {
				root = root->right;
				continue;
			}
		}

	}
	return NULL;
}

//交换
Tree *change(Tree * root, Tree * a, int type)
{
	Tree *ret = root;
	Tree *p=NULL;		//parent of a.
	Tree *b=NULL;
	Tree *c=NULL;
	p = findparent(root, a);
	switch (type) {
	case LL:
		b = a->lift;

		a->lift = b->right;
		b->right = a;
		if (root == a)
			ret = b;
		else {
			if (p->lift == a)
				p->lift = b;
			if (p->right == a)
				p->right = b;
		}

		break;
	case LR:
		//1
		b = a->lift;
		c = b->right;
		//2
		b->right = c->lift;
		a->lift = c->right;
		c->lift = b;
		c->right = a;
		//3
		if (p == NULL)
			ret = c;
		else {
			if (p->lift == a)
				p->lift = c;
			if (p->right == a)
				p->right = c;
		}

		break;
	case RR:
		b = a->right;

		a->right = b->lift;
		b->lift = a;
		if (p == NULL)
			ret = b;
		else {
			if (p->lift == a)
				p->lift = b;
			if (p->right == a)
				p->right = b;
		}
		break;
	case RL:
		b = a->right;
		c = b->lift;
		//
		a->right = c->lift;
		b->lift = c->right;
		c->lift = a;
		c->right = b;
		//
		if (p == NULL)
			ret = c;
		else {
			if (p->lift == a)
				p->lift = c;
			if (p->right == a)
				p->right = c;
		}

		break;
	default:
		printf("err at change\n");
		break;
	}
	return ret;

}

Tree *avl_insert(Tree * root, int value)
{
	int type;
	//Tree *ret = root;
	//1普通插入
	insert_tree(root, value);
	//2更新祖先的平衡因子
	//updatebf(root, value);
	//3 找A点
	Tree *a = NULL;
	a = finda(root, value);
	while (a != NULL) {
		//找到a
		//4确定类型,RRLL等非别用数字表示.
		type = adjust(a);
#if DEBUG
		printf("Insert node is %6d\t", value);
		printf("Find A->data=%6d\t\t", a->data);
		printf("A->bf=%2d\t\t", a->bf);
		printf("type of A is %d\n", type);
#endif
		//5变换6返回
		root = change(root, a, type);
		//仅更新a和其祖先(其兄弟不可能为2/-2)
		//
		//updatebf(root, a->data);
		//再找a
		a = NULL;
		a = finda(root, value);
	}
	//7 没有a了返回.
	return root;
}

int main(int argc, char *argv[])
{
	//三种情况条件编译
#if SMALL
	Tree *root = create_leaf(13);
	root = avl_insert(root, 24);
	root = avl_insert(root, 37);
	root = avl_insert(root, 90);
	root = avl_insert(root, 53);
	root = avl_insert(root, 39);
	root = avl_insert(root, 14);
	root = avl_insert(root, 22);
	root = avl_insert(root, 26);
	root = avl_insert(root, 12);
	root = avl_insert(root, 20);
#endif

#if ORDER
	int i;
	Tree *root = create_leaf(N / 2);
	for (i = 1; i < N; i++)
		root = avl_insert(root, i);
#endif

#if RANDOM
	int i;
	Tree *root = create_leaf(N / 2);
	for (i = 1; i < N; i++)
		root = avl_insert(root, random() % N);
#endif

	printf("len of tree is %d\n", length(root));
	//inorder(root);
	printf("\n");
	//preorder(root);
	return 0;
}
