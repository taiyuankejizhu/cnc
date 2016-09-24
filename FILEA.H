#define KEYWORD_NUM  30
#define DELIMITERS_NUM 7
char far * Program;
unsigned long COMPL01_Adress;
unsigned Word_list,Wrong_Num;
void Analyse();
int find(char *buf,int type);
int cs_manage(char *buffer);
int ch_manage(char *buffer);
void er_manage(char error,int lineno);
void st_manage(unsigned char ErrNo,unsigned sentance);
char scanner();
long stol(char str[]);
char _KeyWords_[]={'G','g','M','m','C','c','H','h','N','n','O','o','P','p','X','x','Y','y','Z','z','R','r','I','i','J','j','K','k','L','l'};
char _Delimiters_[]={'+','-','.','=',';','(',')'};
int F_line;
char Word_temp;
