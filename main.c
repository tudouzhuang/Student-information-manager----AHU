#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*整体思路是先读取文本里面的数据，然后处理数据用链表，每次数据处理完毕都写一次到数据里面，虽然有多余的擦除和写入，
但是整体思路清晰，实现较容易*/

struct Grade {
	int Chinese;
	int Math;
	int English;
};
struct Student {
	char name[50];
	char id[20];
	struct Grade grade;
	struct Student* next;
};
typedef struct Student Student;
typedef struct Grade Grade;
// 定义学生以及成绩结构体

FILE *student_data;
//定义文件的全局变量

HWND Name_input;
HWND ID_input;
HWND g_hInsertButton;
HWND g_hSearchButton;
HWND g_hDeleteButton;
HWND Output_text;
HWND Name_lable;
HWND ID_lable;
HWND Math_lable;
HWND Math_input;
HWND Chinese_lable;
HWND Chinese_input;
HWND English_lable;
HWND English_input;
HWND Grade_input_1;
HWND Grade_input_2;
HWND Grade_lable_1;
HWND Grade_lable_2;
HWND Sort_lable;
HWND Sort_C;
HWND Sort_M;
HWND Sort_E;
HWND Search_lable;
HWND Search_C;
HWND Search_M;
HWND Search_E;
HWND Refresh;
// windows窗口要用的全局变量

// 这个是学生链表的头节点,本身不存储数据
Student* g_head = NULL;
// 这个是求某一范围成绩时单独调用的链表头节点
Student* range_list = NULL;
// 这个全局变量是为了实现每次显示6个节点,按next或者last来显示前或者后6个节点
Student* current_display_node;
// 这个是每次显示的时候,要显示的那6个节点的头节点
Student* display_list;


Student InsertStudent(Student temp_student,Student *head);
void SearchStudent(char* name,char* id);
void DeleteStudent(char* name,char* id);
void UpdateOutputBox(Student* head);
int Init_linklist(Student *head);
void Rewrite_data(Student *head);
void Sort_Student(Student *head,int subject);
int CompareStudent(Student* a, Student* b, int subject);
Student* Find_range(int high,int low,int subject,Student* head,Student* range_list);
int Range_compare(Student* stu,int subject,int high,int low);
int revise(char* name,char* id,Student* head);

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_CREATE:
		/*
		窗口创建说明:
		HWND变量 = CreateWindow(
		"控件类名"  EDIT,BUTTON等
		"初始名称"  自定义
		WS_VISIBLE | WS_CHILD | WS_BORDER   
		（WS_VISIBLE 表示创建后立即显示该控件
		WS_CHILD 表示该控件是子窗口的一部分
		WS_BORDER 表示给窗口添加边框。）
		50, 50  控件左上角相对于父窗口左边框的水平距离（x 坐标）和垂直距离（y 坐标）。
		150, 25  这是控件的宽度和高度
		hwnd  这是指定了父窗口的句柄，即将成为父窗口的子控件。
		NULL：这是关于菜单的句柄，这里不涉及菜单，因此为 NULL。
		NULL：这是控件的实例句柄，这里不需要指定，所以为 NULL。
		NULL：这是传递给控件创建的其他参数，这里也不需要，所以为 NULL。
		*/
		
		// 创建姓名输入框
		Name_input = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 22, 150, 25, hwnd, NULL, NULL, NULL);
		// 创建姓名输入框前的标签
		Name_lable = CreateWindow("STATIC", "Name", WS_VISIBLE | WS_CHILD , 10, 25, 40, 25, hwnd, NULL, NULL, NULL);
		// 创建语文输入框
		Chinese_input = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 22, 150, 25, hwnd, NULL, NULL, NULL);
		// 创建语文输入框前的标签
		Chinese_lable = CreateWindow("STATIC", "Chinese", WS_VISIBLE | WS_CHILD , 240, 25, 55, 25, hwnd, NULL, NULL, NULL);
		// 创建数学输入框
		Math_input = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 62, 150, 25, hwnd, NULL, NULL, NULL);
		// 创建数学输入框前的标签
		Math_lable = CreateWindow("STATIC", "Math", WS_VISIBLE | WS_CHILD , 240, 65, 35, 25, hwnd, NULL, NULL, NULL);
		// 创建英语输入框
		English_input = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 102, 150, 25, hwnd, NULL, NULL, NULL);
		// 创建英语输入框前的标签
		English_lable = CreateWindow("STATIC", "English", WS_VISIBLE | WS_CHILD , 240, 105, 55, 25, hwnd, NULL, NULL, NULL);
		// 创建学号输入框
		ID_input = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 62, 150, 25, hwnd, NULL, NULL, NULL);
		// 创建成绩输入框1
		Grade_input_1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 102, 60, 25, hwnd, NULL, NULL, NULL);
		// 创建成绩输入框2
		Grade_input_2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 102, 60, 25, hwnd, NULL, NULL, NULL);
		// 创建成绩输入框前的标签
		Grade_lable_1 = CreateWindow("STATIC", "Grade", WS_VISIBLE | WS_CHILD , 10, 105, 40, 25, hwnd, NULL, NULL, NULL);
		// 创建成绩输入框中间的标签
		Grade_lable_2 = CreateWindow("STATIC", "----", WS_VISIBLE | WS_CHILD , 115, 105, 20, 25, hwnd, NULL, NULL, NULL);
		// 创建学号输入框前的标签
		ID_lable = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD , 25, 65, 15, 25, hwnd, NULL, NULL, NULL);
		// 创建插入按钮
		g_hInsertButton = CreateWindow("BUTTON", "insert", WS_VISIBLE | WS_CHILD, 30, 150, 100, 25, hwnd, (HMENU)1, NULL, NULL);
		// 创建查询按钮
		g_hSearchButton = CreateWindow("BUTTON", "search", WS_VISIBLE | WS_CHILD, 190, 150, 100, 25, hwnd, (HMENU)2, NULL, NULL);
		// 创建Search_lable
		Search_lable = CreateWindow("STATIC", "Search", WS_VISIBLE | WS_CHILD , 280, 423, 70, 25, hwnd, NULL, NULL, NULL);
		// 创建Search_C按钮
		Search_C = CreateWindow("BUTTON", "C", WS_VISIBLE | WS_CHILD, 330, 420, 20, 25, hwnd, (HMENU)4, NULL, NULL);
		// 创建Search_M按钮
		Search_M = CreateWindow("BUTTON", "M", WS_VISIBLE | WS_CHILD, 350, 420, 20, 25, hwnd, (HMENU)5, NULL, NULL);
		// 创建Search_E按钮
		Search_E = CreateWindow("BUTTON", "E", WS_VISIBLE | WS_CHILD, 370, 420, 20, 25, hwnd, (HMENU)6, NULL, NULL);
		// 创建删除按钮
		g_hDeleteButton = CreateWindow("BUTTON", "delete", WS_VISIBLE | WS_CHILD, 353, 150, 100, 25, hwnd, (HMENU)3, NULL, NULL);
		// 创建sort:的标签
		Sort_lable = CreateWindow("STATIC", "Sort", WS_VISIBLE | WS_CHILD , 148, 423, 30, 25, hwnd, NULL, NULL, NULL);
		// 创建sort Chinese的按钮
		Sort_C = CreateWindow("BUTTON","C", WS_VISIBLE | WS_CHILD, 180, 420, 20, 25, hwnd, (HMENU)7, NULL, NULL);
		// 创建sort Math的按钮
		Sort_M = CreateWindow("BUTTON","M", WS_VISIBLE | WS_CHILD, 200, 420, 20, 25, hwnd, (HMENU)8, NULL, NULL);
		// 创建sort English的按钮
		Sort_E = CreateWindow("BUTTON","E", WS_VISIBLE | WS_CHILD, 220, 420, 20, 25, hwnd, (HMENU)9, NULL, NULL);
		// 创建Main 按钮,返回主页面
		Refresh = CreateWindow("BUTTON","Refresh", WS_VISIBLE | WS_CHILD, 50, 420, 70, 25, hwnd, (HMENU)14, NULL, NULL);
		// 创建输出框
		Output_text = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, 30, 190, 440, 220, hwnd, NULL, NULL, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			//为了减少代码,在search_range的时候给subject赋值用的
			int temp_for_search;
			int temp_for_sort;
			case 1: // 插入按钮被点击
			{
				// 获取姓名输入框中的文本
				char name[50];
				GetWindowText(Name_input, name, sizeof(name));
				// 获取学号输入框中的文本
				char id[20];
				GetWindowText(ID_input, id, sizeof(id));
				//检查是否有与数据库中重复的name或者id
				int check = revise(name,id,g_head);
				// 获取成绩输入框中的三个文本
				char chinese_get[5],math_get[5],english_get[5];
				GetWindowText(Chinese_input,chinese_get,sizeof(chinese_get));
				GetWindowText(Math_input,math_get,sizeof(math_get));
				GetWindowText(English_input,english_get,sizeof(english_get));
				Student student_get;
				Grade grade_get;
				//检测输入信息是否有空的
				if(!strcmp(name,"") || !strcmp(id,"") || !strcmp(chinese_get,"") || !strcmp(math_get,"") || !strcmp(english_get,""))
				{
					SetWindowText(Output_text,"Incomplete information, please complete the information before inserting.\r\n");
					break;
				}
				else
				{
				//转换三个文本到int
					grade_get.Chinese = atoi(chinese_get);
					grade_get.Math = atoi(math_get);
					grade_get.English = atoi(english_get);
					strcpy(student_get.name,name);
					strcpy(student_get.id,id);
					student_get.grade = grade_get;
					
				//根据上面的返回值做修改操作
					if(check == 1)
					{
					//先删除原有的,再插入新的,当作修改
						DeleteStudent(name,id);
						// 插入学生信息
						InsertStudent(student_get,g_head);
						// 写入学生信息
						Rewrite_data(g_head);
						// 更新输出框内容
						UpdateOutputBox(g_head);
					}
					else
					{
						// 插入学生信息
						InsertStudent(student_get,g_head);
						// 写入学生信息
						Rewrite_data(g_head);
						// 更新输出框内容
						UpdateOutputBox(g_head);
					}
				}
				
			}
			break;
			case 2: // 查询按钮被点击
			{
				// 获取姓名输入框中的文本
				char name[50],id[20];
				GetWindowText(Name_input, name, sizeof(name));
				GetWindowText(ID_input, id, sizeof(id));
				//判断信息给的是不是空的
				if(strcmp(name,"")==0 && strcmp(id,"")==0)
				{SetWindowText(Output_text, "Error,haven't input information.");
					return 0;}
				// 查询学生信息
				SearchStudent(name,id);
			}
			break;
			case 3: // 删除按钮被点击
			{
				// 获取姓名输入框中的文本
				char name[50];
				char id[40];
				GetWindowText(Name_input, name, sizeof(name));
				GetWindowText(ID_input, id, sizeof(id));
				if(!strcmp(name,"") && !strcmp(id,""))
				{
					SetWindowText(Output_text,"Incomplete information, please complete the information before deleting.\r\n");
					break;
				}
				// 删除学生信息
				DeleteStudent(name,id);
				Rewrite_data(g_head);
				// 更新输出框内容
				UpdateOutputBox(g_head);
			}
			break;
			case 4: // Search_C按钮被点击
			{
				temp_for_search = 1;
				goto loop;
			}
			case 5: // Search_M按钮被点击
			{
				temp_for_search = 2;
				goto  loop;
			}
			case 6: // Search_E按钮被点击
			{
				temp_for_search = 3;
			}
			
				// 最好是不用goto语句,在这里为了减少重复代码
			loop:	
			{
				char high_0[5],low_0[5];
				int  high,low;
				//获取高低成绩中的信息
				GetWindowText(Grade_input_1, high_0, sizeof(high_0));
				GetWindowText(Grade_input_2, low_0, sizeof(low_0));
				if(!strcmp(high_0,"") || !strcmp(low_0,""))
				{
					SetWindowText(Output_text,"Incomplete information, please complete the Grade range information before searching.\r\n");
					break;
				}
				if(atoi(high_0) >= atoi(low_0))	
				{high = atoi(high_0);
					low = atoi(low_0);}
				else
				{
					high = atoi(low_0);
					low = atoi(high_0);
				}
				UpdateOutputBox(Find_range(high,low,temp_for_search,g_head,range_list));
			}
			break;
			case 7: // Sort_C按钮被点击
			{
				temp_for_sort = 1;
				goto loop_for_sort;
			}
			case 8: // Sort_M按钮被点击
			{
				temp_for_sort = 2;
				goto loop_for_sort;
			}
			case 9: // Sort_E按钮被点击
			{
				temp_for_sort = 3;
			}
			loop_for_sort:
			{
				Sort_Student(g_head,temp_for_sort);
				Rewrite_data(g_head);
				UpdateOutputBox(g_head);
			}
			break;
			case 14: // Refresh按钮被点击
			{
				UpdateOutputBox(g_head);
			}
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

// 插入学生信息
Student InsertStudent(Student temp_student,Student *head) {
	Student* newStudent = (Student*)malloc(sizeof(Student));
	strcpy(newStudent->name, temp_student.name);
	strcpy(newStudent->id, temp_student.id);
	newStudent->grade = temp_student.grade;
	newStudent->next = NULL;
	Student* p = head;
	while(p->next != NULL)
		p = p->next;
	p->next = newStudent;
	return *newStudent;
	//尾插法
	
}

// 查询学生信息
void SearchStudent(char* name,char* id) {
	
	// 在链表中查找姓名为name的学生信息
	Student* current = g_head->next;
	while (current != NULL) 
	{
		if (strcmp(current->name, name) == 0) {
			char output[512];
			sprintf(output, "Name:%s\r\nID:%s\r\nChinese:%d\r\nMath:%d\r\nEnglish:%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
			SetWindowText(Output_text, output);
			return;
		}
		if (strcmp(current->id, id) == 0) {
			char output[512];
			sprintf(output, "Name:%s\r\nID:%s\r\nChinese:%d\r\nMath:%d\r\nEnglish:%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
			SetWindowText(Output_text, output);
			return;
		}
		current = current->next;
	}
	// 没有找到
	SetWindowText(Output_text, "Can't find this student.");
}

// 删除学生信息
void DeleteStudent(char* name,char* id) {
	// 特殊情况：头节点后面的第一个节点就是符合的节点
	if (strcmp(g_head->next->name, name) == 0) {
		Student* temp = g_head->next;
		g_head->next = temp->next;
		free(temp);
		return;
	}
	else if (strcmp(g_head->next->id, id) == 0) {
		Student* temp = g_head->next;
		g_head->next = temp->next;
		free(temp);
		return;
	}
	// 遍历链表找到要删除的节点的前一个节点
	Student* current = g_head;
	while (current->next != NULL) {
		if (strcmp(current->next->name, name) == 0) {
			Student* temp = current->next;
			current->next = current->next->next;
			free(temp);
			return;
		}
		current = current->next;
	}
	current = g_head;
	while (current->next != NULL) {
		if (strcmp(current->next->id, id) == 0) {
			Student* temp = current->next;
			current->next = current->next->next;
			free(temp);
			return;
		}
		current = current->next;
	}
}

//在文本中删除学生的信息
void Rewrite_data(Student *head)
{
	student_data = fopen("student_data.txt","w");
// 先清空文件，然后再根据链表处理的结果重新写入
	student_data = fopen("student_data.txt","a+");
	if (student_data == NULL) {
		SetWindowText(Output_text,"Can't open the file\n");
	}
//打开文件并且检查文件是否存在
	Student *temp = head->next;
	while(temp != NULL)
	{
		fprintf(student_data, "Name:%s\t\tID:%s\t\tGrade.Chinese:%d\t\tGrade.Math:%d\t\tGrade.English:%d\n",
		temp->name,temp->id,temp->grade.Chinese,temp->grade.Math,temp->grade.English);
		temp = temp->next;
//一行太长了，换个行好看点
	}
	fclose(student_data);
	
}


// 更新输出框内容，以表格形式显示学生信息
void UpdateOutputBox(Student* head) {
	// 清空输出框内容
	SetWindowText(Output_text,"");
	int flag=0;
	// 遍历链表，显示所有学生信息
	Student* current = head->next;
	//flag的设置是为了让第一行显示名称
	while (current != NULL) {
		
		//接下来一整段if运用是为了在第一行显示名称,空格和\t混用为了排版整齐
		char output[512];
		if(flag==0)
		{
			sprintf(output,"Name\t\t\tID          Chinese\tMath   English\r\n");
			sprintf(output+strlen(output), "%-20s\t        %s\t%d\t %d\t%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
			flag=1;
		}
		else
		{
			sprintf(output, "%-20s\t        %s\t%d\t %d\t%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
		}
		//这里Name后面\t换成空格是因为如果name后面有两个一样的字母，\t就会被略去，原因不明
		SendMessage(Output_text, EM_REPLACESEL, 0, (LPARAM)output);
		current = current->next;
	}
}



//每次都需要的初始化执行函数
int Init_linklist(Student *head)
{
	student_data = fopen("student_data.txt", "r+");
	if (student_data == NULL) {
		SetWindowText(Output_text,"There's no file named 'student_data.'\r\nNow successfully created a new one.");
		student_data = fopen("student_data.txt", "w+"); 
	// 创建一个新文件
		if (student_data == NULL) {
			SetWindowText(Output_text,"Failed to creat a new file.\n");
			return 1;
		}
	}
    // 文件已存在或者成功创建，继续处理
	char line[120]; // 假设每行最多100个字符
	char name[40],id[20];
	int chinese, math, english;
	// 逐行读取文件内容并解析
	while (fgets(line, sizeof(line), student_data) != NULL) {
		// 使用sscanf解析每行内容
		if (sscanf(line, "Name:%39s \t\t ID:%19s\t\t Grade.Chinese:%d\t\t Grade.Math:%d\t\t Grade.English:%d\n",
			name, id, &chinese, &math, &english) == 5) 
		{
			Student temp_student;
			strcpy(temp_student.name,name);
			strcpy(temp_student.id,id);
			Grade temp_grade;
			temp_grade.Chinese = chinese;
			temp_grade.Math = math;
			temp_grade.English = english;
			temp_student.grade = temp_grade;
			InsertStudent(temp_student,g_head);
		} 
		else {
		SetWindowText(Output_text, "There's an error in reading the file.");
		}
	}
	
	//这里直接抄用update的代码，但是防止之前的信息被刷掉，所以重改一点，不直接调用
	Student* current = g_head->next;
	int flag=0;
	//flag的设置是为了让第一行显示名称
	while (current != NULL) {
		
		//接下来一整段if运用是为了在第一行显示名称,空格和\t混用为了排版整齐
		char output[512];
		if(flag==0)
		{
			sprintf(output,"Name\t\t\tID          Chinese\tMath   English\r\n");
			sprintf(output+strlen(output), "%-20s\t        %s\t%d\t %d\t%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
			flag=1;
		}
		else
		{
			sprintf(output, "%-20s\t        %s\t%d\t %d\t%d\r\n", current->name, current->id,current->grade.Chinese,current->grade.Math,current->grade.English);
		}
		//这里Name后面\t换成空格是因为如果name后面有两个一样的字母，\t就会被略去，原因不明
		SendMessage(Output_text, EM_REPLACESEL, 0, (LPARAM)output);
		current = current->next;
	}
	// 设置滚动条位置到第一行开头
	SendMessage(Output_text, EM_SCROLL, SB_TOP, 0);
	// 关闭文件
	fclose(student_data);
	return 0;
}


//给链表排序用的函数
//subject用来判断比较哪一门科目
void Sort_Student(Student *head, int subject) {
	if (head->next == NULL || head->next->next == NULL)
		return;
	
// 创建一个新的头节点作为已排序链表的头部
	Student *sorted_head = (Student *)malloc(sizeof(Student));
	sorted_head->next = NULL;
	
// 遍历原始链表，依次将节点插入到已排序链表中
	Student *current = head->next;
	while (current != NULL) {
		Student *next_node = current->next; 
// 保存下一个节点的指针
		current->next = NULL;
// 断开当前节点与原链表的连接
		
		// 找到插入位置
		Student *prev = sorted_head;
		while (prev->next != NULL && !CompareStudent(prev->next, current, subject)) {
			prev = prev->next;
		}
		
		// 插入节点
		current->next = prev->next;
		prev->next = current;
		
		// 继续处理下一个节点
		current = next_node;
	}
	
	// 将排序后的链表接入到原始链表中
	head->next = sorted_head->next;
	
	// 释放新建的头节点
	free(sorted_head);
}



// 用于检测是否有重复的名称或者id,同时返回值以供修改信息
int revise(char* name,char* id,Student* head)
{
	Student *temp = head;
	while(temp->next != NULL)
	{
		if(strcmp(name,temp->name) || strcmp(id,temp->id))
			return 1;
		//发现重复的id或者name就返回1,然后进行修改操作
		temp=temp->next;
	}
	return 0;
}

//前面比后面大就返回1,否则返回0
int CompareStudent(Student* a, Student* b, int subject) {
	// 根据指定科目进行比较
	switch (subject) {
		case 1: // Chinese
		return (a->grade.Chinese >= b->grade.Chinese) ? 1 : 0;
		case 2: // Math
		return (a->grade.Math >= b->grade.Math) ? 1 : 0;
		case 3: // English
		return (a->grade.English >= b->grade.English) ? 1 : 0;
	default:
		// 默认按照姓名进行比较
		return strcmp(a->name, b->name) >= 0 ? 1 : 0;
	}
}



Student* Find_range(int high, int low, int subject, Student* head, Student* range_list) {
	Student* p = head->next;
	Student* p_range = range_list;
	
// 清空range_list，释放之前可能存在的节点
	while (p_range != NULL) {
		Student* temp = p_range;
		p_range = p_range->next;
		free(temp);
	}
	
// 初始化range_list
	range_list = NULL;
	
	while (p != NULL) {
		if (Range_compare(p, subject, high, low)) {
			Student* new_student = (Student*)malloc(sizeof(Student));
			if (new_student == NULL) {
// 处理内存分配失败
				SetWindowText(Output_text,"Memory allocation failed.\n");
				exit(1);
			}
			*new_student = *p;
			new_student->next = NULL;
			
			if (range_list == NULL) {
				range_list = new_student;
			} else {
// 将新节点添加到range_list的末尾
				Student* temp = range_list;
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = new_student;
			}
		}
		p = p->next;
	}
	return range_list;
}



int Range_compare(Student* stu,int subject,int high,int low)
{
	switch (subject) {
		case 1: // Chinese
		return (stu->grade.Chinese >= low && stu->grade.Chinese <= high) ? 1 : 0;
		case 2: // Math
		return (stu->grade.Math >= low && stu->grade.Math <= high) ? 1 : 0;
		case 3: // English
		return (stu->grade.English >= low && stu->grade.English <= high) ? 1 : 0;
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//分配头指针内存,防止内存出问题
	g_head = (Student*)malloc(sizeof(Student));
	if (g_head == NULL) {
		fprintf(stderr, "Error memory of head\n");
		return 1;
	}
	g_head->next = NULL;
// 注册窗口类
	
	WNDCLASS wc = {0};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "MyWindowClass";
	RegisterClass(&wc);
	
	// 创建窗口
	HWND hwnd = CreateWindowEx(0, "MyWindowClass", "Students' information manager", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);
	
	// 显示窗口
	ShowWindow(hwnd, nCmdShow);
	Init_linklist(g_head);
	// 消息循环
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}


