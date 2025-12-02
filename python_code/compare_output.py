import subprocess
from cover import find_pmrs, brute_cover
from words import non_isomorphic_binary_words, non_isomorphic_binary_words_upto, nth_fibonacci_word, fibonacci_word_upto, lyndon_words
import time


def convert_output(output):
    """convert repeats.cc output into Python tuples"""
    results = []

    # convert (0,1,2,0) (3,2,3,1) to ["(0,1,2,0)", "(3,2,3,1)"]
    for part in output.strip().split():
        
        # remove ( )
        part = part.strip("()")

        # convert "(0,1,2,0)" to ["0", "1", "2", "0"]
        values = tuple(map(int, part.split(",")))
        results.append(values)
    return results


def compare_pmr(word):
    """compare output primitive maximal repeats"""

    # get output repeats.cc
    result = subprocess.run(
        ["./repeats", word],
        text=True,
        capture_output=True
    )
    cc_output = convert_output(result.stdout)

    # get output cover.py
    py_output = find_pmrs(word)

    # sort output repeats.py and cover.py
    cc_output.sort()
    py_output.sort()

    # check whether output is the same
    if cc_output != py_output:
        print(f"ERROR for word '{word}'")
        print(f"  C++:    {cc_output}")
        print(f"  Python: {py_output}")


def compare_cover(word):
    """compare output of brute force maximal cover"""
    start_cc = time.perf_counter() # start timer

    # get output repeats.cc
    result_cc = subprocess.run(
        ["./repeats", "bruteforce", word],
        text=True,
        capture_output=True
    )

    cc_output = int(result_cc.stdout)

    end_cc = time.perf_counter()
    length = end_cc - start_cc
    hours = int(length // 3600)
    minutes = int((length % 3600) // 60)
    seconds = int(length % 60)
    milliseconds = int((length * 1000) % 1000)
    # print(f"time cc: {hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}")

    start_py = time.perf_counter() # start timer

    pmrs = sorted(find_pmrs(word))
    py_output = brute_cover(word, pmrs)

    end_py = time.perf_counter()
    length = end_py - start_py
    hours = int(length // 3600)
    minutes = int((length % 3600) // 60)
    seconds = int(length % 60)
    milliseconds = int((length * 1000) % 1000)
    # print(f"time py: {hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}")

    # check whether output is the same
    if cc_output != py_output:
        print(f"❌ ERROR for word '{word}'")
        print(f"  C++:    {cc_output}")
        print(f"  Python: {py_output}")
    else:
        print(f"✅ GOODD")
    


def test_greedy(word):
    """output brute force vs greedy algorithms"""

    # get bruteforce output
    result_bf = subprocess.run(
        ["./repeats", "bruteforce", word],
        text=True,
        capture_output=True
    )
    output_bf = int(result_bf.stdout)

    # get greedy output
    result_greedy = subprocess.run(
        ["./repeats", "greedy", word],
        text=True,
        capture_output=True
    )
    output_greedy = int(result_greedy.stdout)

    # check whether output is the same
    if output_bf != output_greedy:
        print(f"❌ ERROR for word '{word}'")
        print(f"  bruteforce: {output_bf}")
        print(f"  greedy    : {output_greedy}")
    else:
        print(f"✅ GOODD")
    
    return output_bf - output_greedy

def test_dynamic(word):
    """output brute force vs greedy algorithms"""
    print(word)

    # The line `print("**********************bruteforce**********************")`
    # is simply printing a visual separator or divider in the output. It is used
    # to visually distinguish or mark a specific section of the output related to
    # the "bruteforce" algorithm. This can help in better organizing and
    # understanding the output when running the program.
    print("**********************bruteforce**********************")
    # get bruteforce output
    result_a = subprocess.run(
        ["./repeats", "bruteforce", word],
        text=True,
        capture_output=True
    )
    # print("*" * 80)
    # lines = result_a.stdout.strip().split("\n")
    # runtime_lines = [line for line in lines if line.startswith("timer")]
    # print(f"{'Word':<15}: {word} - {len(word)}")
    # print(f"{'Word':<15}:  {len(word)}")
    # for line in runtime_lines:
    #     print(f"{' ':<15}{line}")
    # print("_" * 50)


    # output_a = int(result_a.stdout)
    print(result_a.stdout)
    print("**********************dynamic**********************")
    # get greedy output
    result_b = subprocess.run(
        ["./repeats", "dynamic_l", word],
        text=True,
        capture_output=True
    )
    # lines = result_b.stdout.strip().split("\n")
    # runtime_lines = [line for line in lines if line.startswith("timer")]
    # for line in runtime_lines:
    #     print(f"{' ':<15}{line}")

    # output_b = int(result_b.stdout)
    print(result_b.stdout)

    # check whether output is the same
    # if output_a != output_b:
    #     print(f"❌ ERROR for word '{word}'")
    #     print(f"  bruteforce: {output_a}")
    #     print(f"  dynamic    : {output_b}")
    # else:
    #     print(f"✅ GOODD")
    


def calculate_output(error, number_words, total_correct, total_error):
    number_words += 1
    if (error == 0):
        total_correct += 1
    else:
        total_error += error
    return number_words, total_correct, total_error


if __name__ == "__main__":
    # total_error = 0
    # total_correct = 0
    # number_words = 0

    # # test 1: non isomorphic binary words of certain length over binary alphabet--> done (up to length 10)
    # for word in non_isomorphic_binary_words(10):
    #     error = test_greedy(word)
    #     number_words, total_correct, total_error = calculate_output(error, number_words, total_correct, total_error)

    # # test 2: non-isomorphic words up to a certain length over binary alphabet --> done (up to length 10)
    # for word in non_isomorphic_binary_words_upto(10):
    #     error = test_greedy(word)
    #     number_words, total_correct, total_error = calculate_output(error, number_words, total_correct, total_error)

    # # test 3: n-th (finite) Fibonacci word --> done
    # for i in range(1, 6):
    #     error = test_greedy(nth_fibonacci_word(i))
    #     number_words, total_correct, total_error = calculate_output(error, number_words, total_correct, total_error)

    # # test 4: the prefix of a certain length of the infinite Fibonacci word --> done 
    # for i in range(10, 60, 10):
    #     error = test_greedy(fibonacci_word_upto(i))
    #     number_words, total_correct, total_error = calculate_output(error, number_words, total_correct, total_error)

    # # test 5: Lyndon words of a certain length over a ordered alphabet --> done (up to length 10)
    # for word in lyndon_words(10):
    #     error = test_greedy(word)
    #     number_words, total_correct, total_error = calculate_output(error, number_words, total_correct, total_error)


    # print(f"MAE: {total_error/number_words}")
    # print(f"accuracy: {(total_correct/number_words)*100}%")



    # for word in non_isomorphic_binary_words(20):
    #     # compare_cover(word)
    #     test_dynamic(word)
    
    # for word in non_isomorphic_binary_words_upto(10):
    # #     # compare_cover(word)
    #     test_dynamic(word)

    # print("chechpoint 1")

    for i in range(1, 8):
    #     # compare_cover(nth_fibonacci_word(i))
        test_dynamic(nth_fibonacci_word(i))

    # print("chechpoint 2")

    # for i in range(100, 1000, 100):
    # #     # compare_cover(fibonacci_word_upto(i))
    #     test_dynamic(fibonacci_word_upto(i))

    # print("chechpoint 3")

    # # # # test_dynamic(fibonacci_word_upto(10000))

    
    # for word in lyndon_words(10):
    # #     # compare_cover(word)
    #     test_dynamic(word)

