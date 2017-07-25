# Правила

## Файлы

- В `.h` файле реализуются только `inline`-функции и методы шаблонов. Объявления отделяются от реализации стандартным разделителем.
- Вначале каждого файла идет шапка, с данным содержанием:

```cpp
/******************************************************************************
                   Tiny Windows Graphics (TWG) - Version 0.1
                   
	Copyright (C) 2017 Sheprut Ilya
	Contact: optitel223@gmail.com
	License: GPL v2.0.
	
	@file 
	@brief Краткое описание файла.
	
	Подробное описание файла.
	
******************************************************************************/
```

- Защита от вложенностей `.h` файлов реализуется таким образом: если имя файла `twg_buffer.h`, то

```cpp
#ifndef TWG_BUFFER_INCLUDED
#define TWG_BUFFER_INCLUDED
// ...
#endif // TWG_BUFFER_INCLUDED
```

## Код

### Имена

- Имена классов начинаются с большой буквы. Пример: `Bufer`, `Window`.
- Имена функций:
	- Начинаются с глагола. Пример: `get`, `set`, `compute`, `blend`, `render`, `draw`.
	- Начинаются с маленькой буквы. Пример: `create()`, `resize()`, `drawTo()`, `setTaskbarProgress()`.
- Имена переменных начинаются с маленькой буквы. Пример: `objectsCount`.
- Имена закрытых переменных класса начинаются с `m_`, далее оформляется как сказано выше. Пример: `m_buf`, `m_width`, `m_taskbarProgress`.
- Имена статических переменных класса начинаются с s_, далее оформляется как сказано выше. Пример: `s_time`, `sm_progress`.
- Макросы пишутся с большой буквы.
- Особые использования глаголов в именах методов:
	- `get/set` должны быть использованы везде, где осуществляется прямой доступ к атрибуту.
	- `compute` может быть использовано в методах, вычисляющих что-либо.
	- `find` - поиск.
	- `init` - инициализация.
	- `assign` - задание других инициализационных данных, когда объект уже инициализирован.
- При описании имен переменных сначала идет прилагательное, затем существительное: `mainForm`, `widthScale`, `leftScrollbar`.
- При описании массивов\контейнеров следует использовать множественное число: `vector<Point> points`.
- Для переменных, обозначающих число объектов использовать префикс `n`: `nLines`, `nPoints`;
- Суффикс No следует использовать для обозначения номера сущности. Пример: `tableNo`, `employeeNo`.
- Переменным в цикле следует давать имена: `i`, `j`, `k`. Допустимо использовать `x`, `y`, когда идет цикл по координатам.
- Префикс `is` следует использовать только для булевых (логических) переменных и методов. Пример: `isSet`, `isVisible`, `isFinished`, `isFound`, `isOpen`.
	- В некоторых ситуациях префикс `is` лучше заменить на другой: `has`, `can` или `should`. Пример: `bool hasLicense();` `bool canEvaluate();` `bool shouldSort();`.
- Пример симметричных имен: 
	- `get/set`
	- `add/remove`
	- `create/destroy`
	- `start/stop`
	- `insert/delete`
	- `increment/decrement`
	- `begin/end`
	- `first/last`
	- `up/down`
	- `min/max`
	- `next/previous`
	- `old/new`
	- `open/close`
	- `show/hide`
	- `suspend/resume`, и т. д.
- Классам исключений следует добавлять в конце `Exception`.
- При описании типов переменных следует использовать auto, если тип известен.

```cpp
auto something = new MyCustomType;
auto keyEvent = static_cast<QKeyEvent *>(event);
auto myList = QStringList({ "FooThing",  "BarThing" });
auto it = myList.const_iterator();
```

### Особенности оформления

- Строго используются `tab'ы` вместо пробелов. 
- Визуальный размер `tab'ов` равен `4` пробелам.
- Стандартные разделители для разных отступов:

```cpp
//-----------------------------------------------------------------------------
	//-------------------------------------------------------------------------
		//---------------------------------------------------------------------
			//-----------------------------------------------------------------
				//-------------------------------------------------------------
					//---------------------------------------------------------
						//-----------------------------------------------------
							//-------------------------------------------------
```

- Нестандартные разделители для разных отступов:

```cpp
//=============================================================================
	//=========================================================================
		//=====================================================================
			//=================================================================
				//=============================================================
					//=========================================================
						//=====================================================
							//=================================================
```

- Многострочные комментарии пишутся следующим образом:

```cpp
/** Comment first line.
	<- there is tab.
	Comment third line.
	@param можно писать документацию.
 */
```

- Используются сокращения:
	- `buffer` = `buf`
	- `initialization` = `init`
	- `color` = `clr`
	- `string` = `str`
	- `position` = `pos`
	- `source` = `src`
	- `destination` = `dst`
	- `image` = `img`
	- `line` = `ln`
	- `control` = `ctrl`
- Пробелы не ставятся:
	- После имени функции.
	- После открывающей скобки.
	- Перед закрывающей скобкой.
- При описании указателей:
	- Звездочка пишется сразу после типа: `void* buf(void);`, `int* p;`, `(float*)(d)`.
	- При указании переменной аналогично, но не пишется две или более переменных на одной строчке. Нельзя: `char* str, *str2;`.

### Блоки

- При описании `{...}` внутри функции пишется таким образом:

```cpp
while (...) {
	// ...
} 
```

```cpp
if (...) {
	// ...
} else {
	// ...
}
```

```cpp
for (...) {
	// ...
}
```

- При описании пространства имен, функций, классов, скобка `{` находится на новой строке.

```cpp
namespace twg
{
	// ...
}
```
```cpp
class Buffer 
{
	public:
		// ...
	private:
		// ...
}
```

```cpp
void twg::Buffer::drawLine() 
{
	// ...
} 
```

-  Цикл `for` с пустым телом следует оформлять следующим образом:

```cpp
for (initialization; condition; update)
	;
```

- При описании условного оператора с длинными выражениями лучше пользоваться таким методом описания:

```cpp
bool inRectangle = (x >= 0) && (y >= 0) && (x <= 100) && (y <= 100);
bool inCircle = (x-10)*(x-10) + (y-10)*(y-10) <= 25*25;
bool isOddLine = (x % 2) == 0;
if (inRectangle && inCircle || isOddLine) {
	doSomething();
	// ...
}
```

### Классы

- При объявлении класса ни один метод не реализуется в его описании, кроме компактных конструкторов.
- Описание класса начинается с его `public`-раздела.
- И классы и реализация описываются в `namespace`.

### Прочее

- Аббривеатуры в названиях: начинаются с большой буквы, далее идут маленькими буквами. Пример: `saveBmp()`, `exportHtmlSource()`.
- Все находится в пространстве имен `twg`.
- При описании перечислений:
	- Они полностью пишутся заглавными буквами.
	- Каждое с новой строчки.
	- Начинаются с общего слова - имени типа, после которого идет `_`.
	- Пример: 

```cpp	
enum Color {
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE
};
```

- Глобальные переменные всегда пишутся с `::`. Пример: `::system("pause");`.
- Используется `nullptr` вместо `NULL` или `0`.

## Документация

- Перед декларацией каждого класса идет комментарий, описывающий данный класс. Пример:

```cpp
/**
 * @class McEndPointMessage
 * The inter-process message to exchange data between EndPoints.
 * @note AllData = Code (2 first bytes) + Content
 */
class McEndPointMessage { ... }
```

- Перед описанием каждого метода идет описывающий его комментарий, пример:

```cpp
/**
 * Assign all data from buffer.
 * @param [in] iData - buffer where will copy from.
 * @param [in] iSize - size of the iData buffer.
 * @return true if successful, false indicates invalid argument(s).
 * @note The iData buffer is not validated for correctness.
 * @see AnotherFunction()
 * @callgraph
 * @hidecallergraph
 * @example example_test.cpp
 * @author    John Doe
 * @version   4.1a
 * @date      1990-2011
 * @pre       First initialize the system.
 * @bug       Not all memory is freed when deleting an object of this class.
 * @warning   Improper use can crash your application
 * @copyright GNU Public License.
 * @exception <exception-object> { exception description }
 * @note { text }
 * @todo { paragraph describing what is to be done }
 */
bool SetAllDataFromBuffer( t_byte const *iData, size_t iSize );
```
- Имеется возможность документировать некоторые вещи в местах, где они физически не находятся.

```cpp  	
/**
 * @struct to document a C-struct.
 * @union to document a union.
 * @enum to document an enumeration type.
 * @fn to document a function.
 * @var to document a variable or typedef or enum value.
 * @def to document a #define.
 * @typedef to document a type definition.
 * @file to document a file.
 * @namespace to document a namespace.
 */
```

- Пример использования страниц.

```cpp
/*! \mainpage A simple manual

Some general info.

This manual is divided in the following sections:
- \subpage intro
- \subpage advanced "Advanced usage"
*/

//-----------------------------------------------------------

/*! \page intro Introduction
This page introduces the user to the topic.
Now you can proceed to the \ref advanced "advanced section".
*/

//-----------------------------------------------------------

/*! \page advanced Advanced Usage
This page is for advanced users.
Make sure you have first read \ref intro "the introduction".
*/

/*! \page page1 A documentation page
  \tableofcontents
  Leading text.
  \section sec An example section
  This page contains the subsections \ref subsection1 and \ref subsection2.
  For more info see page \ref page2.
  \subsection subsection1 The first subsection
  Text.
  \subsection subsection2 The second subsection
  More text.
*/
/*! \page page2 Another page
  \include <file-name>
*/
```

- Пример использования ссылок.

```cpp
/*! \file autolink.cpp
  Testing automatic link generation.
  
  A link to a member of the Autolink_Test class: Autolink_Test::member, 
  
  More specific links to the each of the overloaded members:
  Autolink_Test::member(int) and Autolink_Test#member(int,int)
  A link to a protected member variable of Autolink_Test: Autolink_Test#var, 
  A link to the global enumeration type #GlobEnum.
 
  A link to the define #ABS(x).
  
  A link to the destructor of the Autolink_Test class: Autolink_Test::~Autolink_Test, 
  
  A link to the typedef ::B.
 
  A link to the enumeration type Autolink_Test::EType
  
  A link to some enumeration values Autolink_Test::Val1 and ::GVal2
*/
```

- Особые возможности форматирования.

```cpp
/**
 * Поддерживаемые HTML-теги:
 * <CENTER>
 * <HR>
 * <SUB>
 * <SUP>
 * <TT>
 * <KBD>
 *
 * Поддерживаемые HTML-символы:
 * &deg;      degree sign: °
 * &plusmn;   plus-minus sign = plus-or-minus sign: ±
 * &copy;     copyright sign: ©
 * &times;    multiplication sign: ×
 * &larr;     leftwards arrow: ←
 * &uarr;     upwards arrow: ↑
 * &rarr;     rightwards arrow: →
 * &darr;     downwards arrow: ↓
 */
```

- Пример использования групп.

```cpp
/**
 * \ingroup A
 */
extern int VarInA;

/**
 * @defgroup IntVariables Global integer variables
 * @brief Brief description
 *
 * Подноброе описание группы.
 *
 * @{
 */

/** an integer variable */
extern int IntegerVariable;

/**@}*/

....

/**
 * \defgroup Variables Global variables
 */
/**@{*/

/** a variable in group A */
int VarInA;

int IntegerVariable;

/**@}*/

/*! \addtogroup mygrp
 *  Additional documentation for group 'mygrp'
 *  @{
 */

/*!
 *  A function
 */
void func1()
{
}

/*! Another function */
void func2()
{
}

 /*! @} */
```

### Использованная литература

- [Некоторые рекомендации по оформлению кода на cpp](http://www.lintest.ru/wiki/%D0%9D%D0%B5%D0%BA%D0%BE%D1%82%D0%BE%D1%80%D1%8B%D0%B5_%D1%80%D0%B5%D0%BA%D0%BE%D0%BC%D0%B5%D0%BD%D0%B4%D0%B0%D1%86%D0%B8%D0%B8_%D0%BF%D0%BE_%D0%BE%D1%84%D0%BE%D1%80%D0%BC%D0%BB%D0%B5%D0%BD%D0%B8%D1%8E_%D0%BA%D0%BE%D0%B4%D0%B0_%D0%BD%D0%B0_C%2B%2B)
- [Habrahabr: 90 рекомендаций по стилю написания программ на cpp](https://habrahabr.ru/post/172091/)
- [Habrahabr: Документируем код эффективно с помощью Doxygen](https://habrahabr.ru/post/252101/)
- [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html)