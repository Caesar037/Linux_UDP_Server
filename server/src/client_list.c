#include "client_list.h"


//初始化节点/链表
node* init_list()
{
    node* new = (node*)malloc(sizeof(node));
    if(new == NULL)
        return NULL;

    new->login = false; //初始化数据
    bzero(new->id, sizeof(new->id));
    bzero(new->password, sizeof(new->password));
    bzero(&new->client_addr, sizeof(new->client_addr));

    INIT_LIST_HEAD(&new->list); //调用内核的初始化接口

    return new;
}

//查找节点
node* find_node(node* head, char* id, char* password)
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    //调用内核的接口来遍历链表
    list_for_each(pos, &head->list)
    {
        p = list_entry(pos, node, list); //p就是遍历出来的结点

        //将结点的数据与要查找的数据进行比较
        if(0 == strcmp(p->id, id) && 0 == strcmp(p->password, password))
        {
            return p;
        }
    }
    return NULL;
}

node* find_node_id(node* head, char* id)
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    //调用内核的接口来遍历链表
    list_for_each(pos, &head->list)
    {
        p = list_entry(pos, node, list); //p就是遍历出来的结点

        //将结点的数据与要查找的数据进行比较
        if(0 == strcmp(p->id, id))
        {
            return p;
        }
    }
    return NULL;
}

//删除节点
int delete_node(node* head, char* id, char* password)
{
    //在接口中调用已设计好的接口
    node* del = NULL;
    del = find_node(head, id, password);

    if(del == NULL)
        return -1; //该链表中没有该数据

    //删除结点
    list_del(&del->list);
    free(del);

    return 0;
}

//插入节点
int insert_node(node* head, char* id, char* password, struct sockaddr_in client_addr)
{
    if(find_node(head, id, password) != NULL)
		return -1; //如果已经存在则返回

    node* new = init_list(); //创建要插入的新结点
    
	strcpy(new->id, id); //给大结构的数据域进行赋值
    strcpy(new->password, password);
    new->login = false;
    new->client_addr = client_addr;

    //调用内核链表的插入标准结点的方法
    list_add(&new->list, &head->list);
    //list_add_tail(&new->list, &head->list);

	return 0;
}

//销毁链表
int destory_list(node* head)
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    list_for_each(pos, &head->list)
    {
        list_del(pos);

        p = list_entry(pos, node, list);
        free(p);
    }

    INIT_LIST_HEAD(&head->list);
    free(head);
	
	return 0;
}

//遍历打印
int print_list(node* head)
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    printf("\n**************用 户 信 息**************\n");
    list_for_each(pos, &head->list)
    {
        p = list_entry(pos,node,list);
        
        printf("node [ id:%s password:%s login:%s ]\n", p->id, p->password, p->login?"true":"false");
    }

	return 0;
}
