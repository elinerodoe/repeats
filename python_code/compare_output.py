import subprocess
from cover import find_pmrs, brute_cover
from words import non_isomorphic_binary_words, non_isomorphic_binary_words_upto, nth_fibonacci_word, fibonacci_word_upto, lyndon_words
import time
import re
import heapq
import subprocess


def improve_output(s):
    matches = list(re.finditer(r"\[(\d+)\]", s))

    for m in reversed(matches):
        waarde = int(m.group(1))
        start, end = m.start(), m.end()

        if waarde == 1:
            s = s[:start] + "." + s[end:]
        else:
            s = s[:end] + "." + s[end:]

    return s


def som_getallen_in_string(s):
    totaal = 0
    vorige_einde = 0

    for m in re.finditer(r"\d+", s):
        waarde = int(m.group())
        if (vorige_einde == 0):
            karakters_ervoor = m.start() - vorige_einde - 1
        else:
            karakters_ervoor = m.start() - vorige_einde - 2

        if (waarde != 1):
            totaal += karakters_ervoor * waarde
        vorige_einde = m.end()

    return totaal


def check_output(sequence, output_HGVS, correct, correct_cover, incorrect_s, incorrect_d):
    output_dynamic = subprocess.run(
        ["./repeats", "dynamic_l", sequence],
        text=True,
        capture_output=True
    )

    output_dynamic_l = output_dynamic.stdout.split("_")
    maximal_cover_dynamic = int(output_dynamic_l[0])
    cover_dynamic = output_dynamic_l[1]

    length_sequence = len(sequence)
    marge = maximal_cover_dynamic * 0.1

    maximal_cover_HGVS = som_getallen_in_string(output_HGVS)
    cover_HGVS = improve_output(output_HGVS)

    print("____________________________________________________________________________________")
    print("sequence: " + sequence)

    if (maximal_cover_dynamic == maximal_cover_HGVS):
        if (cover_dynamic == cover_HGVS): # equal maximal cover and cover
            correct += 1
            print("✅✅ correct")
        else: # equal maximal cover and different cover
            correct_cover += 1
            print("✅ correct - andere cover")
            print(f"output dynamic: {cover_dynamic} ({maximal_cover_dynamic})")
            print(f"output HGVS:    {cover_HGVS} ({maximal_cover_HGVS})")
    else: # different maximal cover
        if (maximal_cover_HGVS >= (maximal_cover_dynamic - marge)): # maximal cover small difference
            incorrect_d += 1
            print("❌/✅ andere cover")
            print(f"output dynamic: {cover_dynamic} ({maximal_cover_dynamic})")
            print(f"output HGVS:    {cover_HGVS} ({maximal_cover_HGVS})")
        else: # maximal cover with high difference
            incorrect_s += 1
            print("❌ slechte cover HGVS")
            print(f"output dynamic: {cover_dynamic} ({maximal_cover_dynamic})")
            print(f"output HGVS:    {cover_HGVS} ({maximal_cover_HGVS})")

    return correct, correct_cover, incorrect_s, incorrect_d 


def exp_mixed(filename):
    total = 0
    count = 0
    correct = 0
    correct_cover = 0
    incorrect_s = 0
    incorrect_d = 0
    with open(filename, "r") as file:
        for line in file:
            total += 1

            # extract sequence from file
            raw_sequence = line.split()[1]
            sequence = raw_sequence.split("/")[1]
            correct_sequence = sequence[:-1]

            # extract output HGVS from file
            raw_output = line.split()[0]
            output = raw_output[15:]
            match = re.match(r"(\d+)(?:_(\d+))?([A-Za-z].*)", output)
            output_HGVS = match.group(3)

            correct, correct_cover, incorrect_s, incorrect_d = check_output(correct_sequence, output_HGVS, correct, correct_cover, incorrect_s, incorrect_d)

    print("total: " + str(total))    
    print("count: " + str(count))
    print("correct: " + str(correct))
    print("correct cover: " + str(correct_cover))
    print("incorrect_s: " + str(incorrect_s))
    print("incorrect_d: " + str(incorrect_d))



def run_algorithm(word, algorithm):
    """output brute force vs greedy algorithms"""

    raw_output = subprocess.run(
        ["./repeats", algorithm, word],
        text=True,
        capture_output=True
    )
    output = raw_output.stdout.split("_")

    print(f"runtime PMR: {float(output[0])}")
    print(f"runtime position: {float(output[1])}")
    print(f"runtime length: {float(output[2])}")
    print(f"runtime dynamic: {float(output[3])}")
    print(f"runtime dynamic_l: {float(output[4])}")

    # return output.stdout
    # return float(output.stdout)


def exp_runtime(algorithm, length_word):
    """run time experiment"""
    total_words = 0
    runtime = 0

    print(f"******************************{algorithm}******************************")

    for word in non_isomorphic_binary_words(length_word):
        total_words += 1
        runtime += run_algorithm(word, algorithm)

    total_words += 1
    runtime += run_algorithm(fibonacci_word_upto(length_word), algorithm)    
    run_algorithm(fibonacci_word_upto(length_word), algorithm)    

    for word in lyndon_words(length_word):
        total_words += 1
        runtime += run_algorithm(word, algorithm)

    print(f"total words: {total_words}")
    print(f"runtime: {runtime}")
    print(f"average runtime per word: {runtime/total_words}\n")


def run_greedy_test(word):
    fout_position = 0
    fout_length = 0
    tot_fout_position = 0
    tot_fout_length = 0

    output = run_algorithm(word, "greedy_position")
    output_greedy = output.split("_")
    maximal_cover_greedy = int(output_greedy[0])
    cover_greedy = output_greedy[1]

    output = run_algorithm(word, "greedy_length")
    output_greedy_l = output.split("_")
    maximal_cover_greedy_l = int(output_greedy_l[0])
    cover_greedy_l = output_greedy_l[1]

    output = run_algorithm(word, "dynamic_l")
    output_dynamic = output.split("_")
    maximal_cover_dynamic = int(output_dynamic[0])
    cover_dynamic = output_dynamic[1]

    if ((maximal_cover_greedy != maximal_cover_dynamic) or (maximal_cover_greedy_l != maximal_cover_dynamic)):
        print("___________________________________________________________")
        print(word)
        print("dynamic: ")
        print("       - maximal cover: " + str(maximal_cover_dynamic))
        print("       - cover: " + str(cover_dynamic))

    if (maximal_cover_greedy != maximal_cover_dynamic):
        fout_position += 1
        tot_fout_position += maximal_cover_dynamic - maximal_cover_greedy
        print("greedy position: ")
        print("       - maximal cover: " + str(maximal_cover_greedy))
        print("       - cover: " + str(cover_greedy))

    if (maximal_cover_greedy_l != maximal_cover_dynamic):
        fout_length += 1
        tot_fout_length += maximal_cover_dynamic - maximal_cover_greedy_l
        print("greedy length: ")
        print("       - maximal cover: " + str(maximal_cover_greedy_l))
        print("       - cover: " + str(cover_greedy_l))
    
    return (fout_position, fout_length, tot_fout_position, tot_fout_length)



def exp_greedy(length_word):
    total_words = 0
    fout_position = 0
    fout_length = 0
    totaal_fout_position = 0
    totaal_fout_length = 0


    for word in non_isomorphic_binary_words(length_word):
        total_words += 1
        fout_pos, fout_len, tot_fout_position, tot_fout_length = run_greedy_test(word)
        fout_position += fout_pos
        fout_length += fout_len
        totaal_fout_position += tot_fout_position
        totaal_fout_length += tot_fout_length

    fout_pos, fout_len, tot_fout_position, tot_fout_length = run_greedy_test(fibonacci_word_upto(length_word))
    total_words += 1
    fout_position += fout_pos
    fout_length += fout_len
    totaal_fout_position += tot_fout_position
    totaal_fout_length += tot_fout_length

    for word in lyndon_words(length_word):
        total_words += 1
        fout_pos, fout_len, tot_fout_position, tot_fout_length = run_greedy_test(word)
        fout_position += fout_pos
        fout_length += fout_len
        totaal_fout_position += tot_fout_position
        totaal_fout_length += tot_fout_length   
    
    print(f"total words: {total_words}")
    print(f"fout position = {fout_position} - gem fout: {totaal_fout_position/fout_position}")
    print(f"fout length = {fout_length} - gem fout: {totaal_fout_length/fout_length}")



if __name__ == "__main__":
    # experiment dbSNP
    exp_mixed("./python_code/mixed_repeats.txt")

    # experiment runtime
    length_words = [10]
    for length_word in length_words:
        print("-----------------------------------------------------------------------------------")
        print(f"Length of words: {length_word}")
        print("-----------------------------------------------------------------------------------")
        exp_runtime("bruteforce", length_word)
        exp_runtime("greedy_position", length_word)
        exp_runtime("greedy_length", length_word)
        exp_runtime("dynamic", length_word)
        exp_runtime("dynamic_l", length_word)
    


    # experiment greedy algorithms
    exp_greedy(10)

