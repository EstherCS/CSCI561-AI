import copy
import re


def is_variable(argument):
    return len(argument) == 1 and argument.islower() and argument.isalpha()


class AtomicSentence:
    def __init__(self, statement, arguments=list(), is_neg=False):
        self.is_neg = is_neg
        self.arguments = arguments
        self.statement = statement

    def neg(self):
        if self.is_neg:
            self.is_neg = False
        else:
            self.is_neg = True
        return copy.deepcopy(self)

    def __eq__(self, other):
        if len(self.arguments) != len(other.arguments):
            return False
        arg_equal = True
        for a, b in zip(self.arguments, other.arguments):
            if a != b:
                arg_equal = False
        return self.is_neg == other.is_neg and arg_equal and self.statement == other.statement

    def __str__(self):
        s = '~' if self.is_neg else ''
        s += self.statement
        s += '('
        for i in range(len(self.arguments) - 1):
            s += self.arguments[i] + ','
        s += self.arguments[-1]
        s += ')'
        return s


class ImplicationSentence:
    def __init__(self, literals, conclusion):
        self.literals = literals
        self.conclusion = conclusion
        self.atomic_sentences = []
        self.to_cnf()

    def to_cnf(self):
        for i in range(len(self.literals)):
            self.literals[i].neg()
        self.atomic_sentences.extend(self.literals)
        self.atomic_sentences.append(self.conclusion)

    def __str__(self):
        s = ''
        for literal in self.literals:
            s += literal.__str__()
            s += '|'
        s += self.conclusion.__str__()
        return s


def get_atomic_sentence(line):
    tokens = re.split(r'[\\(,\\)]', line)
    tokens.remove('')
    # print(tokens)
    for j in range(len(tokens)):
        tokens[j] = tokens[j].strip()
    if tokens[0].find('~') >= 0:
        return AtomicSentence(tokens[0][1:], tokens[1:], is_neg=True)
    else:
        return AtomicSentence(tokens[0], tokens[1:], is_neg=False)


def read_input(path):
    queries = []
    KB = []
    with open(path, 'r') as f:
        num_queries = int(f.readline())
        for i in range(num_queries):
            line = f.readline()
            line = line.replace('\r', '').replace('\n', '')
            queries.append(get_atomic_sentence(line))

        num_sentences = int(f.readline())
        for i in range(num_sentences):
            line = f.readline()
            line = line.replace('\r', '').replace('\n', '')
            if line.find('=>') >= 0:
                tokens = line.split('=>')
                conclusion = get_atomic_sentence(tokens[1].strip())
                ts = tokens[0].split('&')
                literals = []
                for t in ts:
                    literals.append(get_atomic_sentence(t.strip()))
                KB.append(ImplicationSentence(literals, conclusion))
            else:
                KB.append(get_atomic_sentence(line))
    return queries, KB


# check whether two sentences can match
def match(query, sentence):
    if query.is_neg == sentence.is_neg:
        return None
    if query.statement != sentence.statement:
        return None
    if len(query.arguments) != len(sentence.arguments):
        return None
    map = {}
    for i in range(len(query.arguments)):
        if is_variable(sentence.arguments[i]):
            map[sentence.arguments[i]] = query.arguments[i]
        else:
            if query.arguments[i] != sentence.arguments[i]:
                return None
    return map


def match_imp(query, imp):
    for atomic in imp.atomic_sentences:
        map = match(query, atomic)
        if map is not None:
            return map
    return None


def is_contradiction(query, sentence):
    if query.statement != sentence.statement:
        return False
    if len(query.arguments) != len(sentence.arguments):
        return False
    res = True
    for a, b in zip(query.arguments, sentence.arguments):
        if a != b:
            return False
    return res and query.is_neg == sentence.is_neg


def replace_variable(sentence, map):
    for i in range(len(sentence.arguments)):
        if is_variable(sentence.arguments[i]) and sentence.arguments[i] in map:
            sentence.arguments[i] = map[sentence.arguments[i]]


def get_atomic_sentences(KB):
    sen = []
    for sentence in KB:
        if isinstance(sentence, AtomicSentence):
            sen.append(sentence)
    return sen


def ask(query, KB):
    while True:
        last_query = query
        for sentence in KB:
            target_atomic = None
            if isinstance(sentence, AtomicSentence):
                map = match(query, sentence)
                if map is not None:
                    return True
            else:
                for atomic in sentence.atomic_sentences:
                    map = match(query, atomic)
                    if map is not None:
                        target_atomic = atomic
                        break
                if target_atomic is None:
                    continue
                sentence.atomic_sentences.remove(target_atomic)
                if len(sentence.atomic_sentences) == 0:
                    return True
                for atomic in sentence.atomic_sentences:
                    replace_variable(atomic, map)

                # search for next query
                atomic_sens = get_atomic_sentences(KB)
                query = None
                for atomic_sen in atomic_sens:
                    if match_imp(atomic_sen, sentence) is not None:
                        query = atomic_sen
                        break

                if query is None and len(sentence.atomic_sentences) == 1:
                    query = sentence.atomic_sentences[0]
                    sentence.atomic_sentences.clear()
                if query is not None:
                    break
                else:
                    return False
        if query is None:
            return False
        else:
            if last_query == query:
                return False
            continue


if __name__ == '__main__':
    queries, KB = read_input('input-111.txt')
    with open('output.txt', 'w') as f:
        for query in queries:
            q = query.neg()
            f.write(str(ask(q, copy.deepcopy(KB))).upper() + '\n')
