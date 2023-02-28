

// android代码中动态修改约束.java


        //----------------------------------------------------------------
        //https://blog.csdn.net/qq_27489007/article/details/127103850
        ConstraintSet constraintSet = new ConstraintSet();
        ConstraintLayout idConstraintLayout = findViewById(R.id.p1);
        constraintSet.clone(idConstraintLayout);
//        constraintSet.clear(R.id.txt1);
        //设置相对父布局剧中
//        constraintSet.centerHorizontally(R.id.txt1, ConstraintSet.PARENT_ID);
//        constraintSet.centerVertically(R.id.txt1, ConstraintSet.PARENT_ID);

        //可以添加动画
//        TransitionManager.beginDelayedTransition(idConstraintLayout);
//        constraintSet.applyTo(idConstraintLayout);

        //还有其他形式  控件A底部和空间B顶部对其   等同于xml中属性 app:layout_constraintBottom_toTopOf
        //constraintSet.connect(R.id.idTxtA, ConstraintSet.BOTTOM, R.id.idTxtB, ConstraintSet.TOP);
        //constraintSet.connect(R.id.p1, ConstraintSet.BOTTOM, R.id.txt1, ConstraintSet.BOTTOM);
//        constraintSet.connect(R.id.txt1, ConstraintSet.TOP, R.id.p1, ConstraintSet.TOP);
        constraintSet.clear(R.id.txt1, ConstraintSet.LEFT);   //清除左边的约束
        constraintSet.clear(R.id.txt1, ConstraintSet.RIGHT);   //清除右边的约束
        constraintSet.clear(R.id.txt1, ConstraintSet.START);

        //清除左边后右边就完全对齐了，否则是居中。因为左右都有约束，在两者共同作用下居中
        //constraintSet.connect(R.id.txt1, ConstraintSet.RIGHT, R.id.p1, ConstraintSet.RIGHT);
        constraintSet.connect(R.id.txt1, ConstraintSet.RIGHT, R.id.p1, ConstraintSet.RIGHT, 170);  //确实可以用间距

        constraintSet.clear(R.id.txt1, ConstraintSet.TOP);  //top 的情况同理 //即要先清理已存在的约束和相对的约束（左对右，上对下）
        //constraintSet.connect(R.id.txt1, ConstraintSet.BOTTOM, R.id.p1, ConstraintSet.BOTTOM, 170);
        //可以用 ConstraintSet.PARENT_ID 来代替具体的控件 id
        constraintSet.connect(R.id.txt1, ConstraintSet.BOTTOM, ConstraintSet.PARENT_ID, ConstraintSet.BOTTOM, 170);
        //constraintSet.connect(R.id.txt1, ConstraintSet.BOTTOM, R.id.p1, ConstraintSet.TOP, -170);  //目前负值是无用的

        //以下代码配置一个约束集，其中按钮视图的左侧连接到EditText视图的右侧，边距为70dp：
//        constraintSet.connect(R.id.txt1, ConstraintSet.LEFT, R.id.p1, ConstraintSet.RIGHT, -270);

        constraintSet.applyTo(idConstraintLayout);

/*
未证实的

注意：
ConstraintLayout 不支持match_parent属性，但支持wrap_content属性。如果你需要用match_parent，将宽度/高度指定为0dp，然后设置left_toleft,right_toRight为parent即可实现横向充满，同理设置竖向的

作者：枫叶1234
链接：https://www.jianshu.com/p/9b8bc08521e9
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

*/
