文件夹包含以下文件:
const.h       parse.h         random.h    statistics.h
montecarlo.h  parsezmatrix.h  README.txt  zmatrix.h

const.h:
	包含各作业中使用的常量.由于该文件创建的比较晚,只在最后几个作业中用到,只包含pi的数值.

random.h:
	通过c++11的random库定义了一些均匀分布的随机数产生器.以及通过舍选法进行玻尔兹曼分布抽样的产生器.

statistics.h:
	利用list容器定义了一些简单的统计函数,比如计算均值,方差,标准差等,为montecarlo.h提供接口.(最初计划利用模板编写比较泛化的程序,由于知识欠缺导致目前代码内模板的使用完全冗余)

zmatrix.h:
	声明了Zmatrix类,提供了直角坐标,内坐标转换,打印等成员函数,实现部分在shared_src/zmatrix.cpp中.由于开始编写时对问题的理解非常粗浅,以及对c++比较陌生,导致类的结构比较混乱,非常低效,因此只在前三题中使用.

parse.h:
	实现部分在shared_src/parse.cpp中.由于找不到合适的c++ parser库,因此编写了个简易的函数式parser库,提供了一些基础类型的Parser以及简单的Parser Combinator,为parsezmatrix.cpp提供支持.较多的利用了c++11中函数模板的特性.对于仅仅解析作业中的输入不是特别必要.

parsezmatrix.h:
	实现部分在shared_src/parsezmatrix.cpp中.在parse.h的基础上,实现了一些解析内坐标文件的函数.并且定义一些解析一些题目输入中共同的parse pattern的函数.

montecarlo.h:
	后四道题中进行montecarlo抽样模拟的头文件.
	Simulation类,是一个比较通用的类.提供进行给定模拟次数并返回模拟结果(在list容器中)runSimulation,以及返回产生结果名称的getQuantityNames一般性方法.自身定义并不完全,只能作为父类使用,为monteCarlo函数提供接口.
	monteCarlo,该函数对给定的Simulation类进行给定模拟次数和重复次数的模拟,并在给定输出流中输出模拟结果以及误差范围和相应的注释.
	Simulationfort4t5,继承Simulation,根据第四题和第五题的要求对Simulation进一步特化,作为第四题,第五提中相应Simulation的父类.
	Simulationfort6t7,同上.
