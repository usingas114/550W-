# 简单的C++语法检查脚本
import re

def check_syntax(file_path):
    try:
        with open(file_path, 'r', encoding='gbk') as f:
            content = f.read()
        
        # 检查是否有明显的语法错误
        issues = []
        
        # 检查括号是否匹配
        def check_brackets(text, open_bracket, close_bracket, name):
            count = 0
            for i, char in enumerate(text):
                if char == open_bracket:
                    count += 1
                elif char == close_bracket:
                    count -= 1
                    if count < 0:
                        issues.append(f"{name}括号不匹配，在位置{i+1}处有多余的{close_bracket}")
            if count > 0:
                issues.append(f"{name}括号不匹配，缺少{count}个{close_bracket}")
        
        check_brackets(content, '(', ')', '圆')
        check_brackets(content, '{', '}', '花')
        check_brackets(content, '[', ']', '方')
        
        # 检查字符串是否闭合
        in_string = False
        string_char = ''
        for i, char in enumerate(content):
            if not in_string and char in '"\'':
                in_string = True
                string_char = char
            elif in_string and char == string_char and content[i-1] != '\\':
                in_string = False
        if in_string:
            issues.append(f"字符串未闭合")
        
        # 检查注释是否闭合
        in_comment = False
        for i in range(len(content)-1):
            if not in_comment and content[i] == '/' and content[i+1] == '*':
                in_comment = True
            elif in_comment and content[i] == '*' and content[i+1] == '/':
                in_comment = False
        if in_comment:
            issues.append(f"多行注释未闭合")
        
        return issues
        
    except Exception as e:
        return [f"读取文件时出错: {str(e)}"]

# 检查未命名3.cpp文件
issues = check_syntax('D:\\c++项目\\源代码\\550W模拟器\\未命名3.cpp')

if issues:
    print("发现的问题:")
    for issue in issues:
        print(f"- {issue}")
else:
    print("未发现明显的语法错误")
