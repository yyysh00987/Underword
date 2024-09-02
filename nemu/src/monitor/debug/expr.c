#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

uint32_t Regval(char *s,bool *success);
uint32_t eval(int p,int q,bool *success);

int max(int a, int b) {
    return (a > b) ? a : b;
}


enum {
	NOTYPE = 256, 
	num=1,
	ZK=2,
	YK=3,
	NO=4,
	EQ=5,
	NEQ=6,
	AND=7,
	OR=8,
	HEX=9,
	REG=10,

	/* TODO: Add more token types */
	//优先级 ||<&&<!=<==<!<*/<+-

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"[0-9]{1,10}",num},					// 1
	{" +",	NOTYPE},				// spaces 256,NOTYPE位置本来应该是数字，
									//但是上面的enum让NOTYPE=256了
	{"\\+", '+'},					// plus
	{"-+",'-'},                     //-
	{"\\*+",'*'},                   //*
	{"/+",'/'},                     // /
	{"\\(",ZK},						//2
	{"\\)",YK},						//3
	{"==", EQ},						// equal 4
	{"!",'!'},
	{"!=",NEQ},
	{"&&",AND},
	{"\\|\\|",OR},
	{"0x[0-9a-fA-F]{1,8}",HEX},
	{"\\$[a-z]{1,31}",REG},

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type; //类型
	char str[32];//32位
} Token;

Token tokens[32];
int nr_token; //计数到第几个token
int len;


uint32_t Regval(char *s,bool *success){
		int i;
	for(i = 0; i < 8; i ++) { //32
		if(strcmp(regsl[i], s) == 0) {
			return reg_l(i);
		}
	}

	for(i = 0; i < 8; i ++) { //16
		if(strcmp(regsw[i], s) == 0) {
			return reg_w(i);
		}
	}

	for(i = 0; i < 8; i ++) { //8
		if(strcmp(regsb[i], s) == 0) {
			return reg_b(i);
		}
	}

	if(strcmp("eip", s) == 0) {
		return cpu.eip;
	}

		*success = false;
		return 0;
	}


static bool make_token(char *e) {
	int position = 0; //初始化
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') { //遍历直到遇到\0
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) { //遍历每个规则
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position; //匹配项起始位置
				int substr_len = pmatch.rm_eo;//匹配项长度

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len; //更新到匹配项之后的位置
				//此时position在匹配项的下一个位置

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				Token tmpToken;

				switch(rules[i].token_type) {
				// 	default: panic("please implement me");
				// 		break;

				// 	case '+': //C中char->int
				// 	tmpToken.type='+';
				// 	//错写成：nr_token++;但实际上这里还需要把识别出来的token放到数组里面去
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case '-':
				// 	tmpToken.type='-';
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case '*':
				// 	tmpToken.type='*';
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case '/':
				// 	tmpToken.type='/';
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case 1://需要知道是哪一个数字
				// 	tmpToken.type=num;
				// 	strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);//同一个token所以nr_token不++
				// 	tokens[nr_token].str[substr_len] = '\0'; 
				// 	nr_token++;
				// 	break;

				// 	case 2:
				// 	tmpToken.type='(';
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case 3:
				// 	tmpToken.type='*';
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case 4:
				// 	tmpToken.type=4;
				// 	tokens[nr_token ++] = tmpToken;
                //     break;

				// 	case 5:
				// 	tmpToken.type=5;
				// 	strcpy(tokens[nr_token].str,"==");
				// 	tokens[nr_token].str[substr_len] = '\0'; 
                //     break;

				// 	case 6:
				// 	tmpToken.type=6;
				// 	strcpy(tokens[nr_token].str,"!=");
				// 	tokens[nr_token].str[substr_len] = '\0'; 
                //     break;

				// 	case 7:
				// 	tmpToken.type=7;
				// 	strcpy(tokens[nr_token].str,"&&");
				// 	tokens[nr_token].str[substr_len] = '\0'; 
                //     break;

				// 	case 8:
				// 	tmpToken.type=8;
				// 	strcpy(tokens[nr_token].str,"||");
				// 	tokens[nr_token].str[substr_len] = '\0'; 
                //     break;

				// 	case 256:
				// 	break;

				// 	case 9:
				// 	tmpToken.type=9;
				// 	strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
                //     tokens[nr_token].str[substr_len] = '\0'; 
				//     break;

				// 	case 10:
				// 	tmpToken.type=10;
				// 	strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
                //     tokens[nr_token].str[substr_len] = '\0'; 
				// 	break;



				// }
				case '+':
                    case '-':
                    case '*':
                    case '/':
                    case EQ:
                    case NEQ:
                    case AND:
                    case OR:
                    case ZK:
                    case YK:
                        tmpToken.type = rules[i].token_type;
                        tokens[nr_token++] = tmpToken;
                        break;

                    case num:
                        tmpToken.type = num;
                        strncpy(tmpToken.str, substr_start, substr_len);
                        tmpToken.str[substr_len] = '\0';
                        tokens[nr_token++] = tmpToken;
                        break;

                    case HEX:
                        tmpToken.type = HEX;
                        strncpy(tmpToken.str, substr_start, substr_len);
                        tmpToken.str[substr_len] = '\0';
                        tokens[nr_token++] = tmpToken;
                        break;

                    case REG:
                        tmpToken.type = REG;
                        strncpy(tmpToken.str, substr_start, substr_len);
                        tmpToken.str[substr_len] = '\0';
                        tokens[nr_token++] = tmpToken;
                        break;

                    case NOTYPE:
                        break;

                    default:
                        fprintf(stderr, "Unknown token type %d\n", rules[i].token_type);
                        exit(1);


				}
				len=nr_token;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}

	}

	 int Howmanytypes=0;
	int j;
	for( j=0;j<32;j++){
		if(tokens[j].type==0)break;
		else Howmanytypes++;
	}
	int k;
	for(k=0;k<Howmanytypes;k++){//HEX:(str16->int16->int10->str10)
		if(tokens[k].type==9){
			uint32_t hex_val=strtol(tokens[k].str,NULL,16);
			char tmp[32];
			sprintf(tmp,"%d",hex_val);
			strcpy(tokens[k].str,tmp);
			
		}
	}

	for(i=0;i<Howmanytypes;i++){//REG
		if(tokens[i].type==10){
			bool is_success=1;
			bool* success=&is_success;
			
			int reg_val=Regval(tokens[i].str,success);
			if(!success) return 0;
			char reg_val_str[32];
			sprintf(reg_val_str, "%d", reg_val);
			strncpy(tokens[i].str, reg_val_str, sizeof(tokens[i].str) - 1);
			tokens[i].str[sizeof(tokens[i].str) - 1] = '\0';  // 确保字符串以 '\0' 结尾


			
		}
	}

	for(i=0;i<Howmanytypes;i++){//-1:'-'从tokens[i].type->tokens[i+1].str
		if((i==0&&tokens[i].type=='-')||((tokens[i-1].type!=1)&&(tokens[i-1].type=!9)&&(tokens[i-1].type!=10)&&(tokens[i-1].type!=YK)&&(tokens[i].type=='-')))
		{
			tokens[i].type=NOTYPE;
			int j;
			for(j=31;j>0;j--){
				tokens[i+1].str[j]=tokens[i+1].str[j-1];
			}
			tokens[i+1].str[0]='-';
			for(j=0;j<Howmanytypes;j++){
				if(tokens[j].type==NOTYPE){
						Howmanytypes--;
						int u;
						for(u=j+1;u<Howmanytypes;u++){
							tokens[u-1]=tokens[u];
						}

				}
			}
	 }
	}

	for(i=0;i<Howmanytypes;i++){
		//!
		if(tokens[i].type=='!'&&tokens[i].type!=NEQ){
		tokens[i].type=NOTYPE;
		int tmp;
		tmp = atoi(tokens[i + 1].str); 

		if(tmp!=0){
			memset(tokens[i].str,0,sizeof(tokens[i].str));
		}
		else{
			memset(tokens[i].str,0,sizeof(tokens[i].str));
			tokens[i].str[0]='1';
		}
		int j;
		for(j=0;j<Howmanytypes;j++){
				if(tokens[j].type==NOTYPE){
						Howmanytypes--;
						int u;
						for(u=j+1;u<Howmanytypes;u++){
							tokens[u-1]=tokens[u];
						}

				}
			}
		}
	}

		for( i=0;i<Howmanytypes;i++){
			//*->前面不是数字&&后面是10||16
			//int->unint>int*-->*(int*)
			if((i>0&&(tokens[i].type=='*')&&(tokens[i-1].type!=num)&&(tokens[i].type!=HEX)&&(tokens[i].type==num))
		||(i>0&&tokens[i].type=='*'&&tokens[i-1].type!=num&&tokens[i].type!=HEX&&tokens[i].type==HEX)
		||(tokens[i].type=='*'&&i==0)){
			tokens[i].type=NOTYPE;
			
			char tmp[32];
			sprintf(tmp,"%p",(void *)tokens[i].str);
			
			int val=*tmp;
			sprintf(tokens[i+1].str,"%d",val);

			for(j=0;j<Howmanytypes;j++){
				if(tokens[j].type==NOTYPE){

						Howmanytypes--;
						int u;
						for( u=j+1;u<Howmanytypes;u++){
							tokens[u-1]=tokens[u];
						}

				}
			}

		}
		}

	return true; 
}



bool check_parenthese(int l,int r){ 
	//得到括号匹配并且可以计算的式子，可能外面大括号，或者其中小括号
	if(tokens[l].type!='('||tokens[r].type!=')') {
		return 0;
		} //l和r写成0和len了
		else{
	while(l<r){ //错写成<=
		if(tokens[l].type=='(')
		{
			if(tokens[r].type==')'){
				l++,r--;
				continue;
			}
			r--;
		}
		else if (tokens[l].type==')') return 0;
		else l++;
	}
	return 1;
}
}


/*	//op->所在位置
				//上面的check函数排除了非法括号
				//但是（1+1）*(1+1)中
				//括号内部的内容不可能成为op
				//所以除括号之外：优先级 ||<&&<!=<==<!<*
			*/
static int findop(int p,int q,bool *success){
		bool sym= false;
		int op=-1; //指示dominant operator的位置
		int i;
			for (i= q; i>=p; i-- ) {
			// 		if (tokens[i].type == ')') {
            //    // Handle nested parentheses
            // 	int count = 1;
           	// 	 while (count > 0 && i >= p) {
            //          i--;
            //    		 if (tokens[i].type == ')') count++;
					 
            //    		 else if (tokens[i].type == '(') count--;
			// 			}
			// 		}
					 if(tokens[i].type == '(')
           			 {
               			 while(tokens[i].type != ')')
                  			  i ++;
							  if (i > q) {
                    *success = false;
                    return -1; // 添加越界检查
                }
          		  }
					
					else if (!sym && (tokens[i].type == '+' || tokens[i].type == '-' || 
                            tokens[i].type == '*' || tokens[i].type == '/')) {
            // Set the operation position
           				 sym = true;
          			  op = max(op,i);
					  return op;
					  printf("The op's position is %d\n",op);
					}

				}

				if(op==-1){
					*success=false;
					return -1;
			}

			*success=true;
			return op;
				
		}

	uint32_t eval(int p,int q,bool *success){
			
			if(p>q){
				*success=false;
				assert(0);
				return -1;//返回到op不存在
			}

			else if(p==q){//10 16 reg

			uint32_t val;
			switch(tokens[p].type){
				case REG:
				val =Regval(tokens[p].str+1,success);
				if(!*success){return 0;}
				break;

				case num:val=strtol(tokens[p].str,NULL,0); val=(uint32_t)val;break;

				case HEX:val=strtol(tokens[p].str,NULL,0);val=(uint32_t)val;break;



				default:assert(0);

			  }
			*success=1;
			return val;
		 }


			else if(check_parenthese(p,q)==true){
				//throw away the parenthese
				return eval(p+1,q-1,success);
			}
			else{

				int op=findop(p,q,success);
				
				int opType=tokens[op].type;

				uint32_t div1,div2;
				div1=eval(p,op-1,success);

				if(!*success) { return 0; }

				div2=eval(op+1,q,success);

				if(!*success) { return 0; }

				
			switch (opType) 
			{
				case 5:
				return div1==div2;

				case 6:
				return div1!=div2;

				case 7:
				return div1&&div2;

				case 8:
				return div1||div2;

				case '+':
				return div1+div2;

				case '-':
				return div1-div2;

				case '*':
				return div1*div2;

				case '/':
				if(div2==0){
					success=false;
					return 0;
				}
				return div1/div2;

				default :
					 printf("No Op type.");
                assert(0);
			}
				
				}
				/* TODO: Insert codes to evaluate the expression. */
				panic("please implement me");
				return 0;
	  	}
	
	

	uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) { //不是token
		*success = false;
		return 0;
	}
	else {
		
		return eval(0,nr_token-1,success);  //对吗
	}

}

	


