# Dior

A kind of Lisp.

##How to use

```
cd dior & make
```

##support

```lisp
if
cond
set!
define
quote
lambda
begin
set-car!
set-cdr!
car
cdr
list
cons
append
require
eval
number?
string?
boolean?
character?
+
-
*
/
=
>=
<=
>
<
print
println
```

##example

Fibonacci

```lisp
;;第一种递归求fibonacci的方法
(define fib-1
	(lambda (n)
		(cond ((= n 0) 0)
		      ((= n 1) 1)
		      (else (+ (fib-1 (- n 1))
		      	       (fib-1 (- n 2)))))))

;;第二种, 尾递归
(define fib-iter
	(lambda (a b count)
		(if (= count 0)
		    b
		    (fib-iter (+ a b) a (- count 1)))))
(define fib-2
	(lambda (n)
		(fib-iter 1 0 n)))


(fib-2 100)
```

##Todo
`macro` and `gc`