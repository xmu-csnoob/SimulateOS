import sys

def analyze_test_results(log_file):
    with open(log_file, 'r') as f:
        log_content = f.read()
    
    # 检查特定测试输出，验证是否符合预期
    test_passed = True

    if "Disk file validation failed" in log_content:
        print("Disk file validation failed.")
        test_passed = False

    if "Memory test failed" in log_content:
        print("Memory test failed.")
        test_passed = False

    if "CPU test failed" in log_content:
        print("CPU test failed.")
        test_passed = False

    if "Disk I/O test failed" in log_content:
        print("Disk I/O test failed.")
        test_passed = False

    if "Virtual disk test failed" in log_content:
        print("Virtual disk test failed.")
        test_passed = False

    if "Process test failed" in log_content:
        print("Process test failed.")
        test_passed = False

    if test_passed:
        print("All tests passed successfully.")
        sys.exit(0)
    else:
        print("Some tests failed.")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 analyze_test_results.py <log_file>")
        sys.exit(1)
    
    log_file = sys.argv[1]
    analyze_test_results(log_file)
