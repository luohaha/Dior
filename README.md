# Dior

A kind of Lisp.

## How to use

```
cd dior & make
```

## Support

```scheme
if                (if predicate then_value else_value)
cond              (cond (predicated_1 clause_1) ... (else clause_else))
set!              (set! var value)
define            (define var value)
quote             (quote value)
lambda            (lambda (x, y, ...) body)
begin             (begin exp1 exp2 exp3 ...)
set-car!          (set-car! var value)
set-cdr!          (set-cdr! var value)
car
cdr
list
cons
append            (append (list ...) (list ...))
require           (require "filename.dior")
eval              (eval exp)
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

## example

Fibonacci

```scheme
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

## Todo
`macro` and `gc`
