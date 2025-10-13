import subprocess
from cover import find_pmrs
from words import non_isomorphic_binary_words, non_isomorphic_binary_words_upto, nth_fibonacci_word, fibonacci_word_upto, lyndon_words


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
    """compare output repeats.cc with cover.py"""

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

def compare_cover():
    print("hello")


if __name__ == "__main__":
    
    # test 1: non isomorphic binary words of certain length over binary alphabet--> done (up to length 10)
    for word in non_isomorphic_binary_words(10):
        compare_pmr(word)

    # test 2: non-isomorphic words up to a certain length over binary alphabet --> done (up to length 10)
    for word in non_isomorphic_binary_words_upto(10):
        compare_pmr(word)

    # test 3: n-th (finite) Fibonacci word --> done
    compare_pmr(nth_fibonacci_word(1))
    compare_pmr(nth_fibonacci_word(2))
    compare_pmr(nth_fibonacci_word(3))
    compare_pmr(nth_fibonacci_word(4))
    compare_pmr(nth_fibonacci_word(5))
    compare_pmr(nth_fibonacci_word(6))

    # test 4: the prefix of a certain length of the infinite Fibonacci word --> done 
    compare_pmr(fibonacci_word_upto(10))
    compare_pmr(fibonacci_word_upto(20))
    compare_pmr(fibonacci_word_upto(30))
    compare_pmr(fibonacci_word_upto(40))
    compare_pmr(fibonacci_word_upto(50))

    # test 5: Lyndon words of a certain length over a ordered alphabet --> done (up to length 10)
    for word in lyndon_words(10):
      compare_pmr(word)

