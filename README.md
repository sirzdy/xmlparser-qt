## 基于 Qt 的 xml生成工具

![效果图](http://orvm1p4c8.bkt.clouddn.com/o_1c30qdi6u1pt911od1vlj1aps1aa69.png)
    
使用 QtXml 实现

### 背景

在高分屏横飞的今天，使用duilib开发软件需要制作三套皮肤，于是此工具应运而生。现在的你，只需要开发好一套，便可以使用此工具自动生成出三套。

### 功能

1. 根据 一套 XML 生成额外的两套。满足1.0-1.5 1.0-2.0/1.5-1.0 1.5-2.0/2.0-1.0 2.0-1.5
2. 支持  "size"  "caption"  "width"  "height"  "maxwidth"  "maxheight"  "minwidth"  "minheight"  "iconsize"  "itemsize"  "dropboxsize"  "thumbsize"  "itemminwidth"  "itemtextpadding"  "sepheight"  "sepwidth"  "inset"   "padding"  "childpadding"  "textpadding"  "sizebox"  "mininfo"  "maxinfo"  "pos"  "roundcorner"  "borderround"  "bordersize" 属性自动缩放，最小为1
3. 支持 文件头 <?xml version="1.0" encoding="utf-8" standalone="yes"?>
4. 支持文档中包含注释


### 注意事项

* 仅支持规范的 Default value 内部的属性更改。如` <Default name="MenuElement" value="width=&quot;240&quot;" />` ，**两个`&quot;`为一组，每组的第一个后面不要加空格，会出错！**
* 不支持 Default value 内部继续嵌入的单引号。如`<Default name="VScrollBar" value="bknormalimage=&quot;file=&apos;scrollbar\scrollbar.bmp&apos; source=&apos;0,36,16,52&apos; mask=&apos;#FFFF00FF&apos; corner=&apos;3,3,3,3&apos;&quot; />` 内部的 `0,36,16,52` `3,3,3,3`无法识别与计算。

### 操作详解

* **拖拽**或者点击**添加按钮**添加 xml文件，支持多个文件；
* 选取目标文件夹。如果设置该选项，则所有文件生成到该目录下。默认是当前所在的路径，生成的文件会添加 _new；
* 调整转换模式 原始倍率 目标倍率；
* 点击执行按钮执行
* 在下面可以看到 相关的操作信息，对于不太合理的尺寸给予提示，帮助开发者快速进行修改。

