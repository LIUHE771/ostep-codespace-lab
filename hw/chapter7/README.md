# 第 7 章作业：调度模拟（FIFO / SJF / RR）

用 OSTEP 的 `scheduler.py` 对比三种调度策略，指标是**平均响应时间**和**平均周转时间**。每题的过程和推导写在 `analysis.md`，模拟器原始输出在 `data/` 里。

## 结果概况

| 题 | 内容 | 关键数字 |
|---|---|---|
| Q1 | 三个 200 的作业 | FIFO = SJF，平均响应 200 / 周转 400 |
| Q2 | 100/200/300 | FIFO = SJF（负载已排序）平均响应 133.33 / 周转 333.33；乱序 FIFO 涨到 466.67 |
| Q3 | RR 时间片 1 | 平均响应 1 / 周转 465.67，取舍很明显 |
| Q4 | SJF 何时 = FIFO 周转 | 作业等长，或负载已升序排列 |
| Q5 | SJF 何时 = RR 响应 | 等长 + 时间片 ≥ 作业长度（RR 不抢占） |
| Q6 | SJF 响应趋势 | 随作业长度线性涨 |
| Q7 | RR 响应趋势 | 最坏 = (N−1)×Q，平均 = ((N−1)/2)×Q |

## 一句话总结

FIFO 怕 convoy，SJF 管周转、RR 管响应；响应和周转不可兼得。

## 目录结构

```
hw/chapter7/
├── README.md              本文件
├── analysis.md            7 题答案 + 推导
└── data/                  模拟器输出
    ├── q1_fifo.txt        Q1 FIFO
    ├── q1_sjf.txt         Q1 SJF
    ├── q2_fifo.txt        Q2 FIFO
    ├── q2_sjf.txt         Q2 SJF
    ├── q2_fifo_unsorted.txt  乱序 FIFO（convoy 演示）
    ├── q3_rr.txt          Q3 RR q=1
    ├── q4_fifo.txt        Q4 FIFO
    ├── q4_sjf.txt         Q4 SJF
    ├── q5_sjf.txt         Q5 SJF
    ├── q5_rr_q100.txt     Q5/Q7 RR q=100
    ├── q6_sjf_1000_2000_3000.txt  Q6 长负载
    ├── q7_rr_q1.txt       Q7 RR q=1
    ├── q7_rr_q10.txt      Q7 RR q=10
    └── q7_rr_q50.txt      Q7 RR q=50
```

## 复现命令（在 Codespace 根目录）

```bash
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 200,200,200 -p FIFO -c   # Q1
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 100,200,300 -p FIFO -c  # Q2
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 100,200,300 -p RR -q 1 -c  # Q3
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 10,20,30 -p SJF -c      # Q4/Q6
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 100,100,100 -p RR -q 100 -c  # Q5/Q7
python3 ./ext/ostep-homework/cpu-sched/scheduler.py -l 1000,2000,3000 -p SJF -c     # Q6
```

记得带 `-c` 才会输出统计；需要复现时加 `-s <seed>`。
