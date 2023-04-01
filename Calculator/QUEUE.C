//////////////*THIS PROGRAM NOT USES THIS CODE*/

void Qinfix_to_postfix(char expr[], char postexpr[])
{
	int i = 0, j = 0;
	char op; // op에는 스택에 저장될 연산자가 pop되어 대입되어 우선순위가 비교된다.

	init_queue();

	while (expr[i] != '\0') { // 중위표기수식 문자열의 요소를 전부 받는다. 모든 경우(if, else if, else)에 i++가 포함되어 있으므로 반복문을 종료시킬 수 있다.
		if (expr[i] >= '0' && expr[i] <= '9') // 만약 해당 요소가 숫자라면
		{
			do
				postexpr[j++] = expr[i++]; // 일단 postexpr[]문자열에 저장, 
			while (expr[i] >= '0' && expr[i] <= '9'); // 근데 숫자 뒤에 숫자가 오면 2자리 수 이상의 수가 되므로 다시 postexor[]문자열에 저장한다. 
			postexpr[j++] = ' '; // 피연산자 각각을 구분하기 위한 공백 입력
		}

		else if (expr[i] == '.') // 만약 해당 요소가 '.'이라면 이는 숫자 뒤에 들어간'.'의(ex. 2.4의 '.'의 선행 요소는 숫자 2) 실수(소수) 표현을 의미하므로
		{
			j--; // 공백으로 구분되어 버린 postexpr[]안에 들어간 선행 요소인 숫자가 사실은 2.4와 같이 하나의 피연산자이므로 공백 제거 위해 j--;
			postexpr[j++] = '.'; // 공백 자리에 '.'을 넣어준다
			i++;
		}

		else if (expr[i] == '(')
			enqueue(expr[i++]);// expr[i]의 값이 왼쪽괄호 '('일 경우 스택 안에 일단 저장

		else if (expr[i] == ')') {
			while (is_Qempty() == 0) {
				op = dequeue(); // expr[i]의 값이 오른쪽괄호 ')'일 경우 스택 안에 저장되어 있는 값을 pop 후 op에 대입

				if (op == '(') break; // 계속 스택에서 빼면서 스택 안에 먼저 들어가 있었던 '('가 나올 경우 멈춤 () 짝을 찾았기 때문.

				else
				{
					postexpr[j++] = op; // '('연산자 나올 때까지 op에 대입된 값을(스택에서 pop된 값을) postexpr에 저장(그대로 출력되는 것과 같은 의미)
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
				}
			}
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // char형 변수 c에 대입된 값이 사칙연산자일 때
			while (is_Qempty() == 0) {

				op = Qpeek(); // 스택 안에 저장되어 있는 값을 peek 후 op에 대입(top 값은 유지되도록)

				if (precedence(expr[i]) <= precedence(op)) {
					postexpr[j++] = op; // op에 대입된 연산자(스택 안)가 expr[i]의 연산자보다 우선순위 높을 경우 postexpr에 저장
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
					dequeue(); // 이후 pop하여 스택 top에서 op 제거
				}
				else break;
			}
			enqueue(expr[i++]); // expr[]문자열에서 읽어들인 사칙연산자는 스택안으로 저장되어 계속 비교되어 결국 postexpr[j]에 대입되도록 함
		}
		else
			i++; // 모든 경우에 i++을 함으로써 반복문에서 무한루프가 나지 않도록 한다.
	}

	while (is_Qempty() == 0) // 스택에 들어가 있던 모든 연산자들을 스택에서 뺀다.
	{
		postexpr[j++] = dequeue(); // 수업시간에 코딩한 예제에서 바로 메인함수에서 출력할 때 사용한 printf("%c ", pop());과 같은 의미이며,
		postexpr[j++] = ' '; // 공백을 통해 구분되도록 한다
	}
	postexpr[j - 1] = '\0'; // 후위표기수식 맨 마지막에 '\0' 추가하여 문자열의 끝을 알림 마지막에 j++이 되었으므로 j-1번째가 마지막이 됨.
}



int Qcheck_matching(char expr[])
{
	int i = 0, prev; // 함수의 인수 expr[]문자열의 요소 하나하나 읽기 위해 int형 변수 i=0 초기화
	char ch; // 요소 대입용 char 변수 ch선언

	init_queue();

	while (expr[i] != '\0') { // 함수의 인수 expr[]문자열의 요소 끝까지 읽을때까지 반복 
		ch = expr[i++]; // ch에 expr문자열 요소 대입 후 후증가로 expr문자열 요소 모두 읽기

		if (ch == '[' || ch == '(' || ch == '{') // 만약 왼쪽 괄호일 경우
			enqueue(ch); // 일단 스택안으로

		else if (ch == ']' || ch == ')' || ch == '}') { // 오른쪽 괄호를 만날 경우
			if (is_Qempty())
				return 2; // 조건 2 위반 : 오른쪽괄호부터 나와버림

			prev = dequeue(); // 스택 공백X라면 스택 맨 위 요소인 괄호 prev에 대입 

			if ((ch == '[' && prev != ']')

				|| (ch == '(' && prev != ')')

				|| (ch == '{' && prev != '}')) { // 만약 괄호의 쌍이 맞지 않는다면

				return 3; // 조건 3 위반
			}
		}
	}
}

double Qcalc_postfix(char expr[])
{
	int i = 0;

	double val, val1, val2; // 피연산자값 소수이므로, 결과가 소수일 수 있으므로 double형 선언

	init_queue();

	while (expr[i] != '\0') {
		if (expr[i] >= '0' && expr[i] <= '9') { // 0보다크고 9보다 작은 피연산자일경우
			val = 0; // 만약 두자릿수 이상이 나올 때를 대비하여 0으로 초기화
			do {
				val = val * 10 + expr[i] - '0'; // 처음 do while 반복을 돌아갈 때는 해당 문자 값이 double형 그대로 저장, do문이 실행되는 횟수의 자릿수가 된다.
				i++;
			} while (expr[i] >= '0' && expr[i] <= '9'); // 다음에도 숫자가 나오는 경우 n자리만큼 10씩 곱해주어 자릿수맞춤
			val1 = 1; // 만약 소수값이 나올 경우를 대비하여 1로 초기화
			if (expr[i] == '.') {
				i++; // '.'이 나온 이후의 값은 무조건 소숫점 이하이므로 다음 요소가 소숫점 처리될 수 있도록 다음요소 탐색 위해 i++;
				do {
					val1 *= 0.1; // do문이 돌아갈 때마다 10의 마이너스 n승씩 늘어감
					val = val + (expr[i] - '0') * val1; // '.'이 나온 이후의값을 소숫점으로 넘김. do문이 실행되는 횟수가 소숫점 n자릿수가 된다.
					i++;
				} while (expr[i] >= '0' && expr[i] <= '9');
			}
			enqueue(val); // n자릿수, 소숫점 n자릿수 처리를 모두 해준 피연산자 값을 스택에 push
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // 연산자일 경우
			val2 = dequeue(); // 스택 맨 위 피연산자 pop
			val1 = dequeue(); // 그 다음 피연산자 pop
			switch (expr[i]) { // 하여 각 연산자의 경우를 스택안에 push
			case '+': enqueue(val1 + val2); break;
			case '-': enqueue(val1 - val2); break;
			case '*': enqueue(val1 * val2); break;
			case '/': enqueue(val1 / val2); break;
			}
			i++;
		}
		else // 만약 피연산자와 연산자를 구분한 공백을 만났을 경우에는 바로 다음요소로 넘어가 탐색되도록 한다.
			i++;
	}
	return dequeue(); // 결국에 마지막에 pop()되는 값은 수식의 값이 되므로 return
}