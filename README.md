# MxcDB

MxcDB 是一个基于 Level 策略 Lsm-Tree 实现的单机 Key-Value 数据库。

## 主要特点

* **LRU缓存**: 使用最近最少使用（LRU）策略来管理读取缓存，确保热门数据被高效地缓存，同时保证缓存空间被充分利用。
* **布隆过滤器**: 采用布隆过滤器技术来防止缓存穿透问题，避免对不存在的键进行不必要的数据库查询操作。同时，通过写时复制技术来避免对字符串的多次拷贝，提高性能和效率。
* **数据压缩**：利用基于 Snappy 的压缩算法对 SSTable 中的数据进行压缩，减少磁盘空间占用，提高存储效率。
* **无锁跳表**：采用无锁跳表数据结构来实现 MemTable 的高效查找和遍历，提高了读写操作的并发性能。
* **前缀压缩**：对于排序的键进行前缀压缩，减少存储空间占用，缓解写放大问题，提高了存储效率。
* **异步合并**：采用后台线程进行SSTable的异步合并，以减少数据冗余，优化存储结构，提高了存储性能和空间利用率。
* **WAL预写日志**：实现WriteBatch机制，通过批量写入WAL（Write-Ahead Logging）预写日志来实现数据恢复和持久性，减少了同步竞争和I/O次数，提高了写入性能和可靠性。
* **单元测试和性能测试**: 使用GTest编写单元测试，通过benchmark进行性能测试，以确保系统的稳定性和性能优化。

## 安装和运行

软件安装命令以**Arch Linux**为例。

1. 安装第三方依赖：

    ```bash
    sudo pacman -S snappy    # 压缩算法
    sudo pacman -S spdlog    # 高效日志库
    sudo pacman -S gtest3    # 单元测试框架
    sudo pacman -S benchmark # 基准性能测试
    sudo p -S python-crc32c  # 校验和
    ```
    

2. 编译运行：

    ```bash
    mkdir build
    cd build
    cmake ..
    make 
    ./MxcDB
    ```