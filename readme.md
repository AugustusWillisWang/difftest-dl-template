Difftest Dynamic Lib Template
======================

其他 c / cpp 项目可以以动态链接库的形式接入 difftest, 控制仿真执行, 并与 DUT 通讯.

## 流程

**将仿真程序生成为动态链接库.** 要在 Verilator Makefile 执行完后手动生成动态链接库, 需要手动改 Verilator 生成出来的 Makefile 脚本. 目前这个过程是写死在 Makefile 里的. WIP.

```sh
make update-emu # 
```

接下来就可以**在其他程序里加载 difftest 动态库, 控制仿真执行并和 DUT 通讯** (目前只支持共享内存). 

```sh
make example
make test
```