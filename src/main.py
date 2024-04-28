# -*- coding:UTF-8 -*-

if __name__ == "__main__":
    print(os.getcwd())

    # 创建一个 Message 对象
    message = pb.ArrayInt64()

    # 使用 ParseFromString 方法从字符串中解析 Protobuf 数据
    with open("output.txt", "rb") as f:
        message.ParseFromString(f.read())
    
    for data in message.data:
        print(data)
