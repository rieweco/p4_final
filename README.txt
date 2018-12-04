*********************************************************
*         	      README FILE			*
*********************************************************

* I used an FSA Table for the scanner.cpp
* Parser file checks grammer for BNF from project descr.

<BNF>:
	<program> ->      void <vars> <block>
	<block>   ->      start <vars> <stats> stop
	<vars>    ->      empty | var Identifier : Integer <vars>
	<expr>    ->      <A> / <expr> | <A> * <expr> | <A>
	<A>	  ->	  <M> + <A> | <M> - <A> | <M>
	<M>       ->      - <M> |  <R>
	<R>       ->      ( <expr> ) | Identifier | Integer   
	<stats>	  ->      <stat>  <mStat>
	<mStat>   ->      empty |  <stat>  <mStat>
	<stat>    ->      <in> | <out> | <block> | <if> | <loop> | <assign>
	<in>      ->      scan Identifier . 
	<out>     ->      out [ <expr> ] .
	<if>      ->      if ( <expr> <RO> <expr> ) <stat>
	<loop>    ->      loop ( <expr> <RO> <expr> ) <stat>
	<assign>  ->      let Identifier  = <expr> .                                
	<RO>      ->      < | < = | > | > = | = = | =     






TO COMPILE: make

TO RUN: ./frontEnd[filename]

	filename is optional argument.
	files must end in .fs182
	if filename is test.fs182,
	it should be entered without .fs182

	ie. ./frontEnd test


	if no argument is entered, keyboard
	input is read until Ctrl + D.
	Note: must end on a new line. 
	May have to press enter after pasting
	input into console.

	ie. ./frontEnd
		paste in:
			void
			var x : 1 
			var y : 1
			start
  			  var z : 1
  			  start
    			    var w : 1
    			    scan w  .
  			  stop
  			  out [ x + y + z + w ] .
			stop

	must press Enter to go to new line 
	AFTER 'stop'

	Then you can press Ctrl-D to run program. 



TO CLEAN UP EXECUTABLES: make clean

***********************************

