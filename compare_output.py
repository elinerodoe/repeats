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
    # start_cc = time.perf_counter() # start timer

    # get output repeats.cc
    result = subprocess.run(
        ["./repeats", word],
        text=True,
        capture_output=True
    )

    cc_output = int(result.stdout)

    # end_cc = time.perf_counter()
    # length = end_cc - start_cc
    # hours = int(length // 3600)
    # minutes = int((length % 3600) // 60)
    # seconds = int(length % 60)
    # milliseconds = int((length * 1000) % 1000)
    # print(f"time cc: {hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}")

    # start_py = time.perf_counter() # start timer

    pmrs = sorted(find_pmrs(word))
    py_output = brute_cover(word, pmrs)

    # end_py = time.perf_counter()
    # length = end_py - start_py
    # hours = int(length // 3600)
    # minutes = int((length % 3600) // 60)
    # seconds = int(length % 60)
    # milliseconds = int((length * 1000) % 1000)
    # print(f"time py: {hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}")

    # check whether output is the same
    if cc_output != py_output:
        print(f"❌ ERROR for word '{word}'")
        print(f"  C++:    {cc_output}")
        print(f"  Python: {py_output}")
    else:
        print(f"✅ GOODD")
    
    return py_output - cc_output


if __name__ == "__main__":
    
    totaal_fout = 0
    count = 0
    # # test 1: non isomorphic binary words of certain length over binary alphabet--> done (up to length 10)
    # for word in non_isomorphic_binary_words(10):
    #     # compare_pmr(word)
    #     count += 1
    #     totaal_fout += compare_cover(word)
    
    # print(totaal_fout/count)


    # # test 2: non-isomorphic words up to a certain length over binary alphabet --> done (up to length 10)
    for word in non_isomorphic_binary_words_upto(10):
        count += 1
        # compare_pmr(word)
        totaal_fout += compare_cover(word)
    
    print(totaal_fout/count)

    # # test 3: n-th (finite) Fibonacci word --> done
    # compare_pmr(nth_fibonacci_word(1))
    # compare_pmr(nth_fibonacci_word(2))
    # compare_pmr(nth_fibonacci_word(3))
    # compare_pmr(nth_fibonacci_word(4))
    # compare_pmr(nth_fibonacci_word(5))
    # compare_pmr(nth_fibonacci_word(6))

    # compare_cover(nth_fibonacci_word(1))
    # compare_cover(nth_fibonacci_word(2))
    # compare_cover(nth_fibonacci_word(3))
    # compare_cover(nth_fibonacci_word(4))
    # compare_cover(nth_fibonacci_word(5))
    # compare_cover(nth_fibonacci_word(6))



    # # test 4: the prefix of a certain length of the infinite Fibonacci word --> done 
    # compare_pmr(fibonacci_word_upto(10))
    # compare_pmr(fibonacci_word_upto(20))
    # compare_pmr(fibonacci_word_upto(30))
    # compare_pmr(fibonacci_word_upto(40))
    # compare_pmr(fibonacci_word_upto(50))

    # compare_cover(fibonacci_word_upto(10))
    # compare_cover(fibonacci_word_upto(20))
    # compare_cover(fibonacci_word_upto(30))
    # compare_cover(fibonacci_word_upto(40))
    # compare_cover(fibonacci_word_upto(50))
    # compare_cover(fibonacci_word_upto(100))

    # # test 5: Lyndon words of a certain length over a ordered alphabet --> done (up to length 10)
    # for word in lyndon_words(10):
    # #   compare_pmr(word)
    #   compare_cover(word)



