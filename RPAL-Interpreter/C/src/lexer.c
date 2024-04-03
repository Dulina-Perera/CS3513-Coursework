#include "../include/lexer.h"

// Declare global variables to hold compiled regex patterns.
static regex_t* digit_regex;
static regex_t* letter_regex;
static regex_t* punctuation_regex;
static regex_t* whitespace_regex;
static regex_t* comment_regex;
static regex_t* keyword_regex;
static regex_t* identifier_regex;
static regex_t* integer_regex;
static regex_t* operator_regex;
static regex_t* string_regex;


TokenStream* lex(char *input) {
    TokenStream* stream = create_linked_list();

    while (*input != '\0') {
        if (is_whitespace(input)) {
            input = ignore_whitespace(input);
        } else if (*input == '"') {
            input = identify_string(input, stream);
        } else if (is_letter(input)) {
            char* temp = identify_keyword(input, stream);

            if (temp != NULL) {
                input = temp;
            } else {
                input = identify_identifier(input, stream);
            }
        }  else if (is_digit(input)) {
            input = identify_integer(input, stream);
        } else if (is_punctuation(input)) {
            input = identify_punctuation(input, stream);
        } else if (*input == '/') {
            input = ignore_comment(input);
        } else {
            input = identify_operator(input, stream);
        }
    }

    free_regex();
    return stream;
}


const char* token_type_to_string(TokenType type) {
    switch (type)
    {
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case KEYWORD: return "KEYWORD";
        case OPERATOR: return "OPERATOR";
        case PUNCTUATION: return "PUNCTUATION";
        case STRING: return "STRING";
        default:
            fprintf(stderr, "Unknown token type encountered: %d\n", type);
            exit(EXIT_FAILURE);
    }
}


static bool is_digit(char* input) {
    int reti;
    if (digit_regex == NULL) {
        digit_regex = (regex_t *)malloc(sizeof(regex_t));
        if (digit_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(digit_regex, DIGIT_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, digit_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, digit_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(digit_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        return true; // If there is a match, return true.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return false; // If there is no match, return false.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, digit_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, digit_regex, buffer, length);

        fprintf(stderr, buffer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
}

static bool is_letter(char* input) {
    int reti;
    if (letter_regex == NULL) {
        letter_regex = (regex_t *)malloc(sizeof(regex_t));
        if (letter_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(letter_regex, LETTER_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, letter_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, letter_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(letter_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        return true; // If there is a match, return true.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return false; // If there is no match, return false.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, letter_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, letter_regex, buffer, length);

        fprintf(stderr, buffer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
}

static bool is_punctuation(char* input) {
    int reti;
    if (punctuation_regex == NULL) {
        punctuation_regex = (regex_t *)malloc(sizeof(regex_t));
        if (punctuation_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(punctuation_regex, PUNCTUATION_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, punctuation_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, punctuation_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(punctuation_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        return true; // If there is a match, return true.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return false; // If there is no match, return false.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, punctuation_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, punctuation_regex, buffer, length);

        fprintf(stderr, buffer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
}

static bool is_whitespace(char* input) {
    int reti;
    if (whitespace_regex == NULL) {
        whitespace_regex = (regex_t *)malloc(sizeof(regex_t));
        if (whitespace_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(whitespace_regex, WHITESPACE_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, whitespace_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, whitespace_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(whitespace_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        return true; // If there is a match, return true.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return false; // If there is no match, return false.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, whitespace_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, whitespace_regex, buffer, length);

        fprintf(stderr, "%s\n", buffer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
}


static char* ignore_comment(char* input) {
    int reti;
    if (comment_regex == NULL) {
        comment_regex = (regex_t *)malloc(sizeof(regex_t));
        if (comment_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(comment_regex, COMMENT_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
	    {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, comment_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, comment_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
	    }
    }

	regmatch_t match;
	reti = regexec(comment_regex, input, 1, &match, 0); // Execute the regex pattern matching.

	if (reti == _REG_NOERROR)
	{
		return input + match.rm_eo - 1; // If there is a match, return the location of the comment.
	}
	else if (reti == REG_NOMATCH)
	{
		return NULL; // If there is no match, return NULL.
	}
	else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}

static char* ignore_whitespace(char* input) {
    int reti;
    if (whitespace_regex == NULL) {
        whitespace_regex = (regex_t *)malloc(sizeof(regex_t));
        if (whitespace_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(whitespace_regex, WHITESPACE_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, whitespace_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, whitespace_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(whitespace_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        return input + match.rm_eo; // If there is a match, return the location of the whitespace.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, whitespace_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, whitespace_regex, buffer, length);

        fprintf(stderr, buffer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
}


static char* identify_keyword(char* input, TokenStream* stream) {
    int reti;
    if (keyword_regex == NULL) {
        keyword_regex = (regex_t *)malloc(sizeof(regex_t));
        if (keyword_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(keyword_regex, KEYWORD_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, keyword_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, keyword_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(keyword_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so - 1);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = KEYWORD;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the keyword.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
    {
        // If there is an error in executing the regex pattern matching, handle the error.
        size_t length = regerror(reti, comment_regex, NULL, 0);
        char *buffer = (char *)malloc(length);
        (void)regerror(reti, comment_regex, buffer, length);

        fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
    }
}

static char* identify_identifier(char* input, TokenStream* stream) {
    int reti;
    if (identifier_regex == NULL) {
        identifier_regex = (regex_t *)malloc(sizeof(regex_t));
        if (identifier_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(identifier_regex, IDENTIFIER_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, identifier_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, identifier_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(identifier_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so + 1);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = IDENTIFIER;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the identifier.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}

static char* identify_integer(char* input, TokenStream* stream) {
    int reti;
    if (integer_regex == NULL) {
        integer_regex = (regex_t *)malloc(sizeof(regex_t));
        if (integer_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(integer_regex, INTEGER_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, integer_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, integer_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(integer_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so + 1);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = INTEGER;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the integer.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}

static char* identify_operator(char* input, TokenStream* stream) {
    int reti;
    if (operator_regex == NULL) {
        operator_regex = (regex_t *)malloc(sizeof(regex_t));
        if (operator_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(operator_regex, OPERATOR_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, operator_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, operator_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(operator_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so + 1);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = OPERATOR;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the operator.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}

static char* identify_punctuation(char* input, TokenStream* stream) {
    int reti;
    if (punctuation_regex == NULL) {
        punctuation_regex = (regex_t *)malloc(sizeof(regex_t));
        if (punctuation_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(punctuation_regex, PUNCTUATION_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, punctuation_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, punctuation_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(punctuation_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so + 1);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = PUNCTUATION;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the punctuation.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}

static char* identify_string(char* input, TokenStream* stream) {
    int reti;
    if (string_regex == NULL) {
        string_regex = (regex_t *)malloc(sizeof(regex_t));
        if (string_regex == NULL) {
            perror("Failed to allocate memory for regex object.");
            exit(EXIT_FAILURE);
        }

        reti = regcomp(string_regex, STRING_REGEX, REG_EXTENDED); // Compile the regex pattern.
        if (reti != _REG_NOERROR)
        {
            // If there is an error in compiling the regex pattern, handle the error.
            size_t length = regerror(reti, string_regex, NULL, 0);
            char *buffer = (char *)malloc(length);
            (void)regerror(reti, string_regex, buffer, length);

            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(EXIT_FAILURE);
        }
    }

    regmatch_t match;
    reti = regexec(string_regex, input, 1, &match, 0); // Execute the regex pattern matching.

    if (reti == _REG_NOERROR && match.rm_so == 0)
    {
        char* value = (char *)malloc(match.rm_eo - match.rm_so + 1);
        if (value == NULL) {
            perror("Failed to allocate memory for token value.");
            exit(EXIT_FAILURE);
        }

        strncpy(value, input, match.rm_eo - match.rm_so);
        value[match.rm_eo - match.rm_so] = '\0';

        Token* token = (Token *)malloc(sizeof(Token));
        if (token == NULL) {
            perror("Failed to allocate memory for token.");
            exit(EXIT_FAILURE);
        }

        token->value = value;
        token->type = STRING;

        insert_at_end(stream, token);

        return input + match.rm_eo; // If there is a match, return the location of the string.
    }
    else if (reti == REG_NOMATCH || match.rm_so != 0)
    {
        return NULL; // If there is no match, return NULL.
    }
    else
	{
		// If there is an error in executing the regex pattern matching, handle the error.
		size_t length = regerror(reti, comment_regex, NULL, 0);
		char *buffer = (char *)malloc(length);
		(void)regerror(reti, comment_regex, buffer, length);

		fprintf(stderr, buffer);
		free(buffer);
		exit(EXIT_FAILURE);
	}
}


static void free_regex() {
    if (digit_regex != NULL) {
        regfree(digit_regex);
    }

    if (letter_regex != NULL) {
        regfree(letter_regex);
    }

    if (punctuation_regex != NULL) {
        regfree(punctuation_regex);
    }

    if (whitespace_regex != NULL) {
        regfree(whitespace_regex);
    }

    if (comment_regex != NULL) {
        regfree(comment_regex);
    }

    if (identifier_regex != NULL) {
        regfree(identifier_regex);
    }

    if (integer_regex != NULL) {
        regfree(integer_regex);
    }

    if (operator_regex != NULL) {
        regfree(operator_regex);
    }

    if (string_regex != NULL) {
        regfree(string_regex);
    }
};