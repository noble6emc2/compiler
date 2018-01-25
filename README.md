# compiler
**c0文法转MIPS ensemble。文法规则：**
1.＜加法运算符＞ ::= +｜- 
分析：如果不是这两个则不能称为加法运算符，注意每种情况需测全，包括对不同类型标识符的处理（int、char）。
2.＜乘法运算符＞ ::= *｜/  
分析：如果不是这两个则不能称为乘法运算符，注意每种情况需测全。
3.＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
分析：如果不是这几种则不能称为关系运算符。
4.＜字母＞   ::= ＿｜a｜．．．｜z｜A｜．．．｜Z
分析:字母只有这几种，也就是说只有这几种字母能用，需要注意考虑下划线的情况。
5.＜数字＞   ::= ０｜＜非零数字＞
分析: 数字是由非零数字这一非终结符得到的。
6.＜非零数字＞  ::= １｜．．．｜９
分析：非零数字是1到9。
7.＜字符＞::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
分析：由前面定义的4种运算符得来，参与定义因子的文法。
8.＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
分析：字符串中只能有这些规定的ASCII值里面的字符。
9.＜程序＞::=［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞,
分析：这指明了C0整个程序的结构，先定义变量和函数，再在main函数中调用。
10.＜常量说明＞ ::= const＜常量定义＞;{ const＜常量定义＞;}
范例：
分析: 与其他语句像极，常量说明语句用","结束，后面必须跟等号。
11.＜常量定义＞   ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
分析：描述了常量说明的过程，同样类型的常量可以放在一起声明。
12.＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝
13.＜整数＞ ::= ［＋｜－］＜无符号整数＞｜０
分析：定义了无符号整数，之后无符号整数参与定义了整数，正或负号最多只能出现一个，负责描述整数的正负性，注意可以有正0和负0。
14.＜标识符＞ ::=  ＜字母＞｛＜字母＞｜＜数字＞｝
分析：标识符只能由字母开头，而且其中只含数字和字母,注意在此文法中标识符不区分大小写。
15.＜声明头部＞   ::=  int＜标识符＞|char＜标识符＞
分析：声明头部用在了有返回值函数定义中，用来分析函数名和它的返回值类型。
16.＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
分析：可以看出变量的说明和其他的语句一样以";"作为结束符。
17.＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
分析：注意文法中变量定义后面不能跟等号，只能定义1维数组,变量。
18.＜类型标识符＞ ::= int | char
分析：变量在此文法中只能有整型和字符类型。
19.＜有返回值函数定义＞::=＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’
分析：用来分析有返回值函数的代码结构，注意函数在此文法中只能有int和char类型，定义的参数必须是变量
以char和int。
20.＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数表＞‘)’‘{’＜复合语句＞‘}’
分析：用来分析无返回值函数的代码结构，void表示不需要返回值，符合语句可以为空。
21.＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
分析：定义了源代码的顺序，即先定义变量，常量，再进行操作。
22.＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
分析：规定了形参如何定义，注意形参定义可以为空。
23.＜主函数＞::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
分析：定义了主函数格式，注意"()"不能填入形参。
24.＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
分析：规定了表达式的表示方式，开头可以没有正负号。
25.＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
分析：规定了项的表示方式，可以看出表达式中项是以-+号连起来的，
要得到结果必须将完成项中的乘法运算才可以，文法通过这种方式达到了先乘除后加减的效果。
26.＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
分析：由于表达式中可以嵌套表达式，所以采用递归分析。
27.＜语句＞ ::= ＜条件语句＞｜＜循环语句＞｜<情况语句>|‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
分析：定义了该C0文法能执行的语句。注意if和switch语句中单条可以不加大括号，而多条语句必须加大括号,
条件语句、循环语句和情况语句不用分号结束。
28.＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
分析：定义了赋值语句的结构。注意被赋值的对象只能是int、char类型变量或数组。
29. ＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞
分析：定义if语句的规则
30. ＜条件＞ ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
分析：定义条件语句的规则，注意只要非0就为真，所以可以输入表达式进行判断。
31. ＜循环语句＞   ::=  do＜语句＞while ‘(’＜条件＞‘)’
分析：循环语句不管条件是否为负总是会先运行一遍，注意其后不以";"做结尾。
32.＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞ ‘}’
33.＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
34.＜情况子语句＞  ::=  case＜常量＞：＜语句＞
分析：定义了情况语句的结构，情况语句中，switch后面的表达式和case后面的常量只允许出现int和char类型；每个情况子语句执行完毕后，不继续执行后面的情况子语句。 
35.＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
36.＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
分析：定义函数调用语句的结构，注意文法只负责语法分析，
如果输入参数和规定参数个数和种类不是一一对应的，需要语义分析来找出错误。
37.＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
分析：值参表和参数表不同，值参表不能有类型标识符(int,char)，但是可以嵌套别的有返回值函数。
38.＜语句列＞   ::=｛＜语句＞｝
分析：提出了语句列的概念，多个语句组成语句列。注意变量和常量说明语句不在语句中，所以要注意说明语句不能和语句交叉出现。
39.＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
分析：scanf中与现在的c不同的是其中输入的是标识符而不是&标识符。所以该文法的scanf函数
要主动取得标识符所表示变量的地址。
40.＜写语句＞    ::=  printf‘(’＜字符串＞,＜表达式＞‘)’|printf ‘(’＜字符串＞‘)’|printf ‘(’＜表达式＞‘)’
分析：该文法中的printf较为简单，没有转义字符，大大降低了实现的难度，注意最多只能有一个表达式和一个字符串。
41.＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
分析：返回值函数中需要return返回结果。注意返回值的表达式要用括号括起来，无返回值则不用填写。
与标准c不同的是，c0文法的主要差别没有头文件引入、if…else结构、逻辑连接符
转义字符和case语句自动跳转。所以文法相对简单但处理还是要有不同。本编译器在语法分析阶段对分别缺失后面的单双引号的字符和字符串进行容错，同时报warning错误。
