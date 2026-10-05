# OSTEP Ch.4 Homework Answers

## Q1

- Prediction / 预测: 两个进程都是纯 CPU（5:100），没有 I/O，CPU 应该不会闲。PID 0 先跑 5 tick，PID 1 在旁边等着；PID 0 跑完 PID 1 再跑 5 tick。总时间 10，CPU 利用率 100%。
```
Time  PID:0   PID:1   CPU  IOs
1     RUN:cpu READY   1
2     RUN:cpu READY   1
3     RUN:cpu READY   1
4     RUN:cpu READY   1
5     RUN:cpu READY   1
6     DONE    RUN:cpu 1
7     DONE    RUN:cpu 1
8     DONE    RUN:cpu 1
9     DONE    RUN:cpu 1
10    DONE    RUN:cpu 1
```
- Reasoning / 理由: 5:100 表示 5 条指令全部是 CPU 指令，不会发 I/O，所以永远不会 BLOCKED，CPU 一直有事干。
- Verified result / 验证结果: q1.txt 里 Total Time 10, CPU Busy 10 (100.00%), IO Busy 0 (0.00%)
- Analysis / 分析: 猜对了。单核下两个纯 CPU 进程只能轮流用 CPU，一个在跑的时候另一个只能 READY。

## Q2

- Prediction / 预测: PID 0 有 4 条 CPU 指令，先跑 4 tick 结束。PID 1 只有一条 I/O，一次 I/O = 发起 1 tick + 阻塞 5 tick + 完成 1 tick = 7 tick。总时间 4 + 7 = 11，CPU 忙 6 tick，利用率 6/11 ≈ 54.55%。
```
Time  PID:0   PID:1      CPU  IOs
1     RUN:cpu READY      1
2     RUN:cpu READY      1
3     RUN:cpu READY      1
4     RUN:cpu READY      1
5     DONE    RUN:io     1
6     DONE    BLOCKED         1
7     DONE    BLOCKED         1
8     DONE    BLOCKED         1
9     DONE    BLOCKED         1
10    DONE    BLOCKED         1
11*   DONE    RUN:io_done 1
```
- Reasoning / 理由: 我一开始差点只算 BLOCKED 的 5 tick，后来想起来 I/O 指令是三步：RUN:io 发起、BLOCKED 等待、RUN:io_done 处理完成，头尾各 1 tick，所以是 7。PID 1 阻塞的时候 PID 0 已经结束了，没有别的进程可跑，那 5 tick CPU 只能闲着。
- Verified result / 验证结果: Total Time 11, CPU Busy 6 (54.55%), IO Busy 5 (45.45%)，和预测一致。
- Analysis / 分析: CPU 空闲的时间正好是 PID 1 阻塞、又没人可运行的那 5 tick。如果这时候有个就绪进程，CPU 就不会闲了。

## Q3

- Prediction / 预测: 和 Q2 只是交换了顺序，这次 PID 0 先发 I/O。默认 SWITCH_ON_IO 会在阻塞时切到 PID 1，PID 1 的 4 条 CPU 指令正好把 5 tick 的等待填掉。总时间 1 + 5 + 1 = 7，CPU 忙 6 tick，约 85.71%。
```
Time  PID:0   PID:1      CPU  IOs
1     RUN:io  READY      1
2     BLOCKED RUN:cpu    1    1
3     BLOCKED RUN:cpu    1    1
4     BLOCKED RUN:cpu    1    1
5     BLOCKED RUN:cpu    1    1
6     BLOCKED DONE            1
7*    RUN:io_done DONE   1
```
- Reasoning / 理由: 跟 Q2 比唯一变化就是顺序。这次 PID 0 等待 I/O 的时候 PID 1 能跑，CPU 就不用空了。
- Verified result / 验证结果: Total Time 7, CPU Busy 6 (85.71%), IO Busy 5 (71.43%)，和预测一致。
- Analysis / 分析: 顺序很重要。Q2 是 11 tick，这里变成 7 tick，就是因为 I/O 等待期间 CPU 有活干了，I/O 和 CPU 重叠了。

## Q4

- Prediction / 预测: SWITCH_ON_END 只在进程结束才切换。PID 0 阻塞的那 5 tick 没人接管，CPU 空转；等 PID 0 整个结束，PID 1 才跑 4 tick。总时间 1 + 5 + 1 + 4 = 11，利用率 6/11 ≈ 54.55%。
```
Time  PID:0   PID:1   CPU  IOs
1     RUN:io  READY   1
2     BLOCKED READY        1
3     BLOCKED READY        1
4     BLOCKED READY        1
5     BLOCKED READY        1
6     BLOCKED READY        1
7*    RUN:io_done READY 1
8     DONE    RUN:cpu 1
9     DONE    RUN:cpu 1
10    DONE    RUN:cpu 1
11    DONE    RUN:cpu 1
```
- Reasoning / 理由: 选项名字写得很直白，END 才切换，所以等 I/O 的时候不会切走，PID 1 只能干等。
- Verified result / 验证结果: Total Time 11, CPU Busy 6 (54.55%), IO Busy 5 (45.45%)，和预测一致。
- Analysis / 分析: 这个策略下 CPU 白浪费 5 tick，比 SWITCH_ON_IO 差。等 I/O 不切换的话，CPU 时间就浪费了。

## Q5

- Prediction / 预测: SWITCH_ON_IO 本来就是默认行为，所以结果应该和 Q3 一样：总时间 7，CPU 85.71%。
```
Time  PID:0   PID:1      CPU  IOs
1     RUN:io  READY      1
2     BLOCKED RUN:cpu    1    1
3     BLOCKED RUN:cpu    1    1
4     BLOCKED RUN:cpu    1    1
5     BLOCKED RUN:cpu    1    1
6     BLOCKED DONE            1
7*    RUN:io_done DONE   1
```
- Reasoning / 理由: 跟 Q4 比只是换了调度策略：I/O 一发出就切到 PID 1，等待时间被 PID 1 的 CPU 指令用掉。
- Verified result / 验证结果: Total Time 7, CPU Busy 6 (85.71%), IO Busy 5 (71.43%)，和 Q3 完全一样。
- Analysis / 分析: Q4（END）11 tick 对 Q5（IO）7 tick，明显"阻塞就切换"更好，CPU 利用率从 54.55% 升到 85.71%。

## Q6

- Prediction / 预测: IO_RUN_LATER 的意思是 PID 0 的 I/O 完成后不急着跑，等自然切换。前 16 tick 三个 CPU 进程（各 5 条）依次跑完，顺便把第一次 I/O 等待也盖住了；之后只剩 PID 0，它每次 I/O 等待 5 tick 都没人用 CPU。数下来总时间 31，CPU 忙 21 tick，约 67.74%。
```
Time  PID:0   PID:1   PID:2   PID:3   CPU  IOs
1     RUN:io  READY   READY   READY   1
2     BLOCKED RUN:cpu READY   READY   1    1
3     BLOCKED RUN:cpu READY   READY   1    1
4     BLOCKED RUN:cpu READY   READY   1    1
5     BLOCKED RUN:cpu READY   READY   1    1
6     BLOCKED RUN:cpu READY   READY   1    1
7*    READY   DONE    RUN:cpu READY   1
8     READY   DONE    RUN:cpu READY   1
9     READY   DONE    RUN:cpu READY   1
10    READY   DONE    RUN:cpu READY   1
11    READY   DONE    RUN:cpu READY   1
12    READY   DONE    DONE    RUN:cpu 1
13    READY   DONE    DONE    RUN:cpu 1
14    READY   DONE    DONE    RUN:cpu 1
15    READY   DONE    DONE    RUN:cpu 1
16    READY   DONE    DONE    RUN:cpu 1
17    RUN:io_done DONE DONE    DONE    1
18    RUN:io  DONE    DONE    DONE    1
19    BLOCKED DONE    DONE    DONE            1
20    BLOCKED DONE    DONE    DONE            1
21    BLOCKED DONE    DONE    DONE            1
22    BLOCKED DONE    DONE    DONE            1
23    BLOCKED DONE    DONE    DONE            1
24*   RUN:io_done DONE DONE    DONE    1
25    RUN:io  DONE    DONE    DONE    1
26    BLOCKED DONE    DONE    DONE            1
27    BLOCKED DONE    DONE    DONE            1
28    BLOCKED DONE    DONE    DONE            1
29    BLOCKED DONE    DONE    DONE            1
30    BLOCKED DONE    DONE    DONE            1
31*   RUN:io_done DONE DONE    DONE    1
```
- Reasoning / 理由: 这题我状态表画了两遍才数对。关键是前 16 tick CPU 一直有人跑，从第 17 tick 开始只剩 PID 0，它的 I/O 轮次里 CPU 就闲了。
- Verified result / 验证结果: Total Time 31, CPU Busy 21 (67.74%), IO Busy 15 (48.39%)，和预测一致。
- Analysis / 分析: LATER 的坏处是 I/O 设备和 CPU 没对上：前面 CPU 进程跑的时候设备闲着，后面设备忙的时候 CPU 闲着，两头都在浪费。

## Q7

- Prediction / 预测: IO_RUN_IMMEDIATE 的话，PID 0 每次 I/O 一完成马上接着跑、马上发下一个 I/O；每次等待期间刚好有一个 CPU 进程在跑（PID 1 → PID 2 → PID 3）。三轮全部重叠，总时间 21，CPU 利用率应该能到 100%。
```
Time  PID:0   PID:1   PID:2   PID:3   CPU  IOs
1     RUN:io  READY   READY   READY   1
2     BLOCKED RUN:cpu READY   READY   1    1
3     BLOCKED RUN:cpu READY   READY   1    1
4     BLOCKED RUN:cpu READY   READY   1    1
5     BLOCKED RUN:cpu READY   READY   1    1
6     BLOCKED RUN:cpu READY   READY   1    1
7*    RUN:io_done DONE READY   READY   1
8     RUN:io  DONE    READY   READY   1
9     BLOCKED DONE    RUN:cpu READY   1    1
10    BLOCKED DONE    RUN:cpu READY   1    1
11    BLOCKED DONE    RUN:cpu READY   1    1
12    BLOCKED DONE    RUN:cpu READY   1    1
13    BLOCKED DONE    RUN:cpu READY   1    1
14*   RUN:io_done DONE DONE    READY   1
15    RUN:io  DONE    DONE    READY   1
16    BLOCKED DONE    DONE    RUN:cpu 1    1
17    BLOCKED DONE    DONE    RUN:cpu 1    1
18    BLOCKED DONE    DONE    RUN:cpu 1    1
19    BLOCKED DONE    DONE    RUN:cpu 1    1
20    BLOCKED DONE    DONE    RUN:cpu 1    1
21*   RUN:io_done DONE DONE    DONE    1
```
- Reasoning / 理由: 我是跟 Q6 对比着想的。IMMEDIATE 让 I/O 进程立刻回来继续发下一个 I/O，这样每一轮等待都有 CPU 进程能填上，不会出现两边都闲着。
- Verified result / 验证结果: Total Time 21, CPU Busy 21 (100.00%), IO Busy 15 (71.43%)，真的 100%。
- Analysis / 分析: Q6 的 31 tick 变成这里 21 tick。对 I/O 多的进程，I/O 一完成就让它继续，CPU 和 I/O 设备都能保持忙碌，改进很明显。

## Q8

- Prediction / 预测: 三个 seed 的指令序列（不加 -c 先跑一遍就能看到）：
  - seed 1: PID 0 = cpu, io, io；PID 1 = cpu, cpu, cpu
  - seed 2: PID 0 = io, io, cpu；PID 1 = cpu, io, io
  - seed 3: PID 0 = cpu, io, cpu；PID 1 = io, io, cpu
- Reasoning / 理由: 先根据指令列表推：I/O 等待的时候有没有 CPU 进程能跑，决定总时间（SWITCH_ON_IO 能重叠，SWITCH_ON_END 不能）；I/O 完成后什么时候让这个进程回来，也会影响后面几轮的重叠。
- Verified result / 验证结果: 每个 seed 跑三种设置，结果如下：

| 设置 | seed 1 | seed 2 | seed 3 |
|---|---|---|---|
| default（SWITCH_ON_IO + IO_RUN_LATER） | 15 tick, CPU 53.33% | 16 tick, CPU 62.50% | 18 tick, CPU 50.00% |
| -I IO_RUN_IMMEDIATE | 15 tick, CPU 53.33% | 16 tick, CPU 62.50% | 17 tick, CPU 52.94% |
| -S SWITCH_ON_END | 18 tick, CPU 44.44% | 30 tick, CPU 33.33% | 24 tick, CPU 37.50% |

- Analysis / 分析:
  - SWITCH_ON_END 明显最差：每次 I/O 等待 CPU 都空着，seed 2 甚至到 30 tick，利用率只有 33%。
  - IO_RUN_IMMEDIATE 和 LATER 在 seed 1、2 里结果一样，因为 I/O 完成的时候没有别的进程在等着抢 CPU；seed 3 里 IMMEDIATE 好一点（18 → 17），PID 0 第二次 I/O 一完成就接着跑，省了 1 tick。
  - 结论：I/O 多的负载用 SWITCH_ON_IO + IO_RUN_IMMEDIATE 最合适，CPU 和 I/O 设备都能充分利用。
