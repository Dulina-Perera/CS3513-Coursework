#include "../include/cse_machine.h"

size_t env_cnt = 0;

static CtrlCell *current_cell = NULL;
static CtrlCell *current_env = NULL;

void init_cse_machine(Vertex *vertex)
{
	current_cell = alloc_ctrl_cell();
	current_env = alloc_ctrl_cell();

	CtrlCell *c1 = alloc_ctrl_cell_with_type(ENV);

	c1->content.env->id = env_cnt++;
	c1->content.env->rename_rules = NULL;
	c1->content.env->prev = NULL;

	current_env = c1;

	CtrlCell *ctrl_structs = generate_ctrl_structs(vertex, true);

	c1->next = ctrl_structs;
	c1->prev = ctrl_structs->prev;

	ctrl_structs->prev->next = c1;
	ctrl_structs->prev = c1;

	current_cell = c1->prev;
}

void eval_cse_machine(void)
{
	if (current_cell->type == IDENTIFIER)
	{
		HashTableEntry *entry = search(current_env->content.env->rename_rules, current_cell->content.s);

		if (entry == NULL)
		{
			current_cell = current_cell->prev;
		}
		else
		{
			CtrlCell *cell = alloc_ctrl_cell_with_type(entry->type);

			if (entry->type == INTEGER)
			{
				add_content(cell, entry->val.i);
			}
			else if (entry->type == STRING)
			{
				add_content(cell, strdup(entry->val.s));
			}
			else if (entry->type == DOUBLE)
			{
				add_content(cell, entry->val.d);
			}
			else if (entry->type == LAMBDA)
			{
				cell->content.lambda = entry->val.lambda;
			}

			cell->prev = current_cell->prev;
			cell->next = current_cell->next;

			free_ctrl_cell(current_cell);
			current_cell = NULL;

			cell->prev->next = cell;
			cell->next->prev = cell;

			current_cell = cell->prev;
		}
	}
	else if (
			current_cell->type == INTEGER ||
			current_cell->type == STRING ||
			current_cell->type == DOUBLE)
	{
		current_cell = current_cell->prev;
	}
	else if (
			current_cell->type == B_OR ||
			current_cell->type == B_AND ||
			current_cell->type == B_NOT)
	{
		if (current_cell->next->type == R_TRUE)
		{
			if (current_cell->type == B_NOT)
			{
				current_cell->next->type = R_FALSE;
			}
			else if (
					current_cell->type == B_OR ||
					current_cell->next->next->type == R_TRUE)
			{
				current_cell->next->next = current_cell->next->next->next;

				free_ctrl_cell(current_cell->next->next->prev);

				current_cell->next->next->prev = current_cell->next;
			}
			else
			{
				current_cell->next = current_cell->next->next;

				free_ctrl_cell(current_cell->next->prev);

				current_cell->next->prev = current_cell;
			}
		}
		else
		{
			if (current_cell->type == B_NOT)
			{
				current_cell->next->type = R_TRUE;
			}
			else if (
					current_cell->type == B_AND ||
					current_cell->next->next->type == R_FALSE)
			{
				current_cell->next->next = current_cell->next->next->next;

				free_ctrl_cell(current_cell->next->next->prev);

				current_cell->next->next->prev = current_cell->next;
			}
			else
			{
				current_cell->next = current_cell->next->next;

				free_ctrl_cell(current_cell->next->prev);

				current_cell->next->prev = current_cell;
			}
		}

		current_cell->prev->next = current_cell->next;
		current_cell->next->prev = current_cell->prev;

		CtrlCell *temp = current_cell;
		current_cell = current_cell->prev;
		free_ctrl_cell(temp);
	}
	else if (
			current_cell->type == B_GR ||
			current_cell->type == B_GE ||
			current_cell->type == B_LS ||
			current_cell->type == B_LE ||
			current_cell->type == B_EQ ||
			current_cell->type == B_NE)
	{
		double a, b;
		
		if (current_cell->next->type == INTEGER)
		{
			a = (double)current_cell->next->content.i;
		}
		else if (current_cell->next->type == DOUBLE)
		{
			a = current_cell->next->content.d;
		}

		if (current_cell->next->next->type == INTEGER)
		{
			b = (double)current_cell->next->next->content.i;
		}
		else if (current_cell->next->next->type == DOUBLE)
		{
			b = current_cell->next->next->content.d;
		}

		CtrlCell *cell = alloc_ctrl_cell();
		if (current_cell->type == B_GR)
		{
			if (a > b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}
		else if (current_cell->type == B_GE)
		{
			if (a >= b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}
		else if (current_cell->type == B_LS)
		{
			if (a < b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}
		else if (current_cell->type == B_LE)
		{
			if (a <= b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}
		else if (current_cell->type == B_EQ)
		{
			if (a == b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}
		else if (current_cell->type == B_NE)
		{
			if (a != b)
			{
				cell->type = R_TRUE;
			}
			else
			{
				cell->type = R_FALSE;
			}
		}

		cell->prev = current_cell->prev;
		cell->next = current_cell->next->next->next;

		current_cell->prev->next = cell;
		current_cell->next->next->next->prev = cell;

		free_ctrl_cell(current_cell->next->next);
		free_ctrl_cell(current_cell->next);
		free_ctrl_cell(current_cell);

		current_cell = cell->prev;
	}
	else if (
			current_cell->type == A_ADD ||
			current_cell->type == A_SUB ||
			current_cell->type == A_MUL ||
			current_cell->type == A_DIV ||
			current_cell->type == A_EXP)
	{
		CtrlCell *cell = alloc_ctrl_cell();

		if (current_cell->next->type == INTEGER && current_cell->next->next->type == INTEGER)
		{
			cell->type = INTEGER;
			add_content(cell, calculate(current_cell->next->content.i, current_cell->next->next->content.i, phrase_type_to_string(current_cell->type)));
		}
		else if (current_cell->next->type == INTEGER && current_cell->next->next->type == DOUBLE)
		{
			cell->type = DOUBLE;
			add_content(cell, calculate(current_cell->next->content.i, current_cell->next->next->content.d, phrase_type_to_string(current_cell->type)));
		}
		else if (current_cell->next->type == DOUBLE && current_cell->next->next->type == INTEGER)
		{
			cell->type = DOUBLE;
			add_content(cell, calculate(current_cell->next->content.d, current_cell->next->next->content.i, phrase_type_to_string(current_cell->type)));
		}
		else if (current_cell->next->type == DOUBLE && current_cell->next->next->type == DOUBLE)
		{
			cell->type = DOUBLE;
			add_content(cell, calculate(current_cell->next->content.d, current_cell->next->next->content.d, phrase_type_to_string(current_cell->type)));
		}

		cell->prev = current_cell->prev;
		cell->next = current_cell->next->next->next;

		current_cell->prev->next = cell;
		current_cell->next->next->next->prev = cell;

		free_ctrl_cell(current_cell->next->next);
		free_ctrl_cell(current_cell->next);
		free_ctrl_cell(current_cell);

		current_cell = cell->prev;
	}
	else if (current_cell->type == A_NEG)
	{
		CtrlCell *cell = current_cell->next;

		if (cell->type == INTEGER)
		{
			add_content(cell, -cell->content.i);
		}
		else if (cell->type == DOUBLE)
		{
			add_content(current_cell->next, -current_cell->next->content.d);
		}

		current_cell->prev->next = current_cell->next;
		current_cell->next->prev = current_cell->prev;

		free_ctrl_cell(current_cell);
		current_cell = NULL;

		current_cell = cell->prev;
	}
	else if (current_cell->type == R_GAMMA)
	{
		if (current_cell->next->type == IDENTIFIER)
		{
			CtrlCell *cell = current_cell->next;

			if (strncmp(cell->content.s, "Print", 5) == 0)
			{
				if (cell->next->type == TAU)
				{
					Tau *tau = cell->next->content.tau;
					for (size_t i = 0; i < tau->expr_cnt; i++)
					{
						if (tau->expressions[i]->type == INTEGER)
						{
							printf("%d ", tau->expressions[i]->content.i);
						}
						else if (tau->expressions[i]->type == STRING)
						{
							printf("%s", tau->expressions[i]->content.s);
						}
						else if (tau->expressions[i]->type == DOUBLE)
						{
							printf("%lf\n", tau->expressions[i]->content.d);
						}
					}
					printf("\n");
				}
				else if (cell->next->type == INTEGER)
				{
					printf("%d\n", cell->next->content.i);
				}
				else if (cell->next->type == STRING)
				{
					printf("%s\n", cell->next->content.s);
				}
				else if (cell->next->type == DOUBLE)
				{
					printf("%lf\n", cell->next->content.d);
				}

				current_cell = current_cell->prev;

				current_cell->next = cell->next->next;
				cell->next->next->prev = current_cell;
			}
		}
		else if (current_cell->next->type == LAMBDA)
		{
			Lambda *lambda = current_cell->next->content.lambda;

			CtrlCell *c1 = alloc_ctrl_cell_with_type(ENV);
			CtrlCell *c2 = alloc_ctrl_cell_with_type(ENV);

			c1->content.env->id = env_cnt++;
			c2->content.env->id = c1->content.env->id;

			if (lambda->param_cnt == 1)
			{
				c1->content.env->rename_rules = init_hash_table(1);

				HashTableEntry *entry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
				if (entry == NULL)
				{
					perror("Memory allocation failed.\n");
					exit(EXIT_FAILURE);
				}

				entry->key = lambda->params[0];
				entry->type = current_cell->next->next->type;
				if (entry->type == INTEGER)
				{
					entry->val.i = current_cell->next->next->content.i;
				}
				else if (entry->type == IDENTIFIER || entry->type == STRING)
				{
					entry->val.s = strdup(current_cell->next->next->content.s);
				}
				else if (entry->type == DOUBLE)
				{
					entry->val.d = current_cell->next->next->content.d;
				}
				else if (entry->type == LAMBDA)
				{
					entry->val.lambda = current_cell->next->next->content.lambda;
				}

				insert(c1->content.env->rename_rules, entry);
			}
			else
			{
				c1->content.env->rename_rules = init_hash_table(2 * lambda->param_cnt);

				HashTableEntry *entry;
				for (int i = 0; i < lambda->param_cnt; ++i)
				{
					entry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
					if (entry == NULL)
					{
						perror("Memory allocation failed.\n");
						exit(EXIT_FAILURE);
					}

					entry->key = lambda->params[0];
					entry->type = current_cell->next->next->type;
					if (entry->type == INTEGER)
					{
						entry->val.i = current_cell->next->next->content.i;
					}
					else if (entry->type == IDENTIFIER || entry->type == STRING)
					{
						entry->val.s = strdup(current_cell->next->next->content.s);
					}
					else if (entry->type == DOUBLE)
					{
						entry->val.d = current_cell->next->next->content.d;
					}
					else if (entry->type == LAMBDA)
					{
						entry->val.lambda = current_cell->next->next->content.lambda;
					}

					insert(c1->content.env->rename_rules, entry);
				}
			}

			c1->content.env->prev = current_env;

			c1->prev = current_cell->prev;
			c1->next = current_cell->next->content.lambda->body;

			c2->prev = current_cell->next->content.lambda->body->prev;
			c2->next = current_cell->next->next->next;

			current_cell->next->content.lambda->body->prev->next = c2;
			current_cell->next->content.lambda->body->prev = c1;

			current_cell->prev->next = c1;
			c2->next->prev = c2;

			current_cell = c2->prev;
			current_env = c1;
		}
	}
	else if (
			current_cell->type == ENV &&
			current_cell->content.env->id != 0)
	{
		CtrlCell *temp;
		if (current_cell->next->type == ENV)
		{
			current_cell->next->next->prev = current_cell->prev;
			current_cell->prev->next = current_cell->next->next;
		}
		else
		{
			temp = current_cell->next->next;

			temp->next->prev = temp->prev;
			temp->prev->next = temp->next;

			current_cell->next->prev = current_cell->prev;
			current_cell->prev->next = current_cell->next;
		}

		current_cell = current_cell->prev;
		current_env = current_env->content.env->prev;
	}
	else if (current_cell->type == BETA)
	{
		CtrlCell *temp;
		if (current_cell->next->type == R_TRUE) {
			current_cell = current_cell->prev->prev;

			temp = current_cell->next;

			current_cell->next->next->next->next->prev = current_cell;
			current_cell->next = current_cell->next->next->next->next;

			free_ctrl_cell(temp->next->next);
			free_ctrl_cell(temp->next);
			free_ctrl_cell(temp);
		}
		else
		{
			current_cell = current_cell->prev;

			temp = current_cell->prev;

			current_cell->prev->prev->next = current_cell;
			current_cell->prev = current_cell->prev->prev;

			free_ctrl_cell(temp);

			temp = current_cell->next;

			current_cell->next->next->next->prev = current_cell;
			current_cell->next = current_cell->next->next->next;

			free_ctrl_cell(temp->next);
			free_ctrl_cell(temp);
		}

		CtrlCell *cell = current_cell->content.delta->cell;
		temp = cell->prev;

		current_cell->prev->next = cell;
		cell->prev = current_cell->prev;

		current_cell->next->prev = temp;
		temp->next = current_cell->next;

		current_cell->content.delta->cell = NULL;
		free_ctrl_cell(current_cell);

		current_cell = temp;
	}
	else if (current_cell->type == TAU)
	{
		CtrlCell *temp;

		Tau *tau = current_cell->content.tau;

		temp = current_cell->next;
		for (size_t i = 0; i < tau->expr_cnt; i++)
		{
			tau->expressions[i] = temp;
			temp = temp->next;
		}

		current_cell->next = temp;
		temp->prev = current_cell;

		current_cell = current_cell->prev;
	}
	else if (current_cell->type == LAMBDA)
	{
		current_cell = current_cell->prev;
	}

	if (
			current_cell->type != ENV ||
			current_cell->content.env->id != 0)
	{
		eval_cse_machine();
	}
}

static CtrlCell *generate_ctrl_structs(Vertex *vertex, bool selfish)
{
	if (vertex == NULL)
	{
		return NULL;
	}
	else
	{
		if (vertex->type == T_COND)
		{
			CtrlCell *_then = alloc_ctrl_cell_with_type(DELTA);
			CtrlCell *_else = alloc_ctrl_cell_with_type(DELTA);
			CtrlCell *beta = alloc_ctrl_cell_with_type(BETA);

			CtrlCell *B = generate_ctrl_structs(get_left_child(vertex), true);
			CtrlCell *T = generate_ctrl_structs(get_right_sibling(get_left_child(vertex)), true);
			CtrlCell *E = generate_ctrl_structs(get_right_sibling(get_right_sibling(get_left_child(vertex))), true);

			_then->content.delta->cell = T;
			_else->content.delta->cell = E;
			beta->content.delta = NULL;

			_then->next = _else;
			_else->prev = _then;

			_else->next = beta;
			beta->prev = _else;

			beta->next = B;
			_then->prev = B->prev;
			B->prev->next = _then;
			B->prev = beta;

			if (!selfish)
			{
				CtrlCell *temp = _then->prev;
				CtrlCell *right_sibling = generate_ctrl_structs(get_right_sibling(vertex), false);
				if (right_sibling != NULL)
				{
					_then->prev = right_sibling->prev;
					right_sibling->prev->next = _then;
					right_sibling->prev = temp;
					temp->next = right_sibling;
				}
			}

			return _then;
		}
		else if (vertex->type == T_TAU)
		{
			CtrlCell *cell = alloc_ctrl_cell_with_type(TAU), *t1, *t2;
			Vertex *v1;

			size_t expr_cnt = 1;
			v1 = get_right_sibling(get_left_child(vertex));
			while (v1 != NULL)
			{
				expr_cnt++;
				v1 = get_right_sibling(v1);
			}
			cell->content.tau->expr_cnt = expr_cnt;

			CtrlCell **expressions = (CtrlCell **)malloc(expr_cnt * sizeof(CtrlCell *));
			if (expressions == NULL)
			{
				perror("Memory allocation failed.\n");
				exit(EXIT_FAILURE);
			}
			cell->content.tau->expressions = expressions;

			cell->prev = cell;
			cell->next = cell;

			v1 = get_left_child(vertex);
			while (v1 != NULL)
			{
				t1 = cell->prev;
				t2 = generate_ctrl_structs(v1, true);

				cell->prev = t2->prev;
				t2->prev->next = cell;
				t2->prev = t1;
				t1->next = t2;

				v1 = get_right_sibling(v1);
			}

			if (!selfish)
			{
				t1 = cell->prev;
				CtrlCell *right_sibling = generate_ctrl_structs(get_right_sibling(vertex), false);
				if (right_sibling != NULL)
				{
					cell->prev = right_sibling->prev;
					right_sibling->prev->next = cell;
					right_sibling->prev = t1;
					t1->next = right_sibling;
				}
			}

			return cell;
		}
		else if (vertex->type == E_LAMBDA)
		{
			CtrlCell *cell = alloc_ctrl_cell_with_type(LAMBDA);
			Vertex *temp;

			size_t param_cnt = 1;
			char **params;
			temp = get_left_child(vertex);
			if (temp->type == V_COMMA)
			{
				temp = get_right_sibling(get_left_child(temp));
				while (temp != NULL)
				{
					param_cnt++;
					temp = temp->right_sibling;
				}

				params = (char **)malloc(param_cnt * sizeof(char *));
				if (params == NULL)
				{
					perror("Memory allocation failed.\n");
					exit(EXIT_FAILURE);
				}

				temp = get_left_child(get_left_child(vertex));
				for (int i = 0; i < param_cnt; ++i)
				{
					params[i] = strdup(temp->token->value.s);
					temp = get_right_sibling(temp);
				}
			}
			else
			{
				params = (char **)malloc(sizeof(char *));
				if (params == NULL)
				{
					perror("Memory allocation failed.\n");
					exit(EXIT_FAILURE);
				}

				params[0] = strdup(get_left_child(vertex)->token->value.s);
			}

			cell->content.lambda->param_cnt = param_cnt;
			cell->content.lambda->params = params;
			cell->content.lambda->body = generate_ctrl_structs(get_right_sibling(get_left_child(vertex)), true);

			cell->prev = cell;
			cell->next = cell;

			if (!selfish)
			{
				CtrlCell *right_sibling = generate_ctrl_structs(get_right_sibling(vertex), false);
				if (right_sibling != NULL)
				{
					cell->prev = right_sibling->prev;
					right_sibling->prev->next = cell;
					cell->next = right_sibling;
					right_sibling->prev = cell;
				}
			}

			return cell;
		}
		else
		{
			CtrlCell *cell = alloc_ctrl_cell();

			if (vertex->type == NONE)
			{
				if (vertex->token->type == INTEGER)
				{
					cell->content.i = vertex->token->value.i;
				}
				else if (vertex->token->type == IDENTIFIER || vertex->token->type == STRING)
				{
					cell->content.s = strdup(vertex->token->value.s);
				}
				cell->type = vertex->token->type;
			}
			else
			{
				cell->type = vertex->type;
			}

			cell->prev = cell;
			cell->next = cell;

			CtrlCell *left_child = generate_ctrl_structs(get_left_child(vertex), false);
			if (left_child != NULL)
			{
				cell->prev = left_child->prev;
				left_child->prev->next = cell;
				cell->next = left_child;
				left_child->prev = cell;
			}

			if (!selfish)
			{
				CtrlCell *t1 = cell->prev;
				CtrlCell *right_sibling = generate_ctrl_structs(get_right_sibling(vertex), false);
				if (right_sibling != NULL)
				{
					cell->prev = right_sibling->prev;
					right_sibling->prev->next = cell;
					right_sibling->prev = t1;
					t1->next = right_sibling;
				}
			}

			return cell;
		}
	}
}

static CtrlCell *alloc_ctrl_cell(void)
{
	CtrlCell *cell = (CtrlCell *)malloc(sizeof(CtrlCell));
	if (cell == NULL)
	{
		perror("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	return cell;
}

static CtrlCell *alloc_ctrl_cell_with_type(size_t type)
{
	CtrlCell *cell = alloc_ctrl_cell();

	cell->type = type;

	if (type == ENV)
	{
		cell->content.env = (Env *)malloc(sizeof(Env));
		if (cell->content.env == NULL)
		{
			perror("Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (type == DELTA)
	{
		cell->content.delta = (Delta *)malloc(sizeof(Delta));
		if (cell->content.delta == NULL)
		{
			perror("Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (type == TAU)
	{
		cell->content.tau = (Tau *)malloc(sizeof(Tau));
		if (cell->content.tau == NULL)
		{
			perror("Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (type == LAMBDA)
	{
		cell->content.lambda = (Lambda *)malloc(sizeof(Lambda));
		if (cell->content.lambda == NULL)
		{
			perror("Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
	}

	return cell;
}

static void add_env_content(CtrlCell *cell, Env *env)
{
	cell->content.env = env;
}

static void add_lambda_content(CtrlCell *cell, Lambda *lambda)
{
	cell->content.lambda = lambda;
}

static void add_delta_content(CtrlCell *cell, Delta *delta)
{
	cell->content.delta = delta;
}

static void add_tau_content(CtrlCell *cell, Tau *tau)
{
	cell->content.tau = tau;
}

static void add_string_content(CtrlCell *cell, char *s)
{
	cell->content.s = s;
}

static void add_int_content(CtrlCell *cell, int i)
{
	cell->content.i = i;
}

static void add_double_content(CtrlCell *cell, double d)
{
	cell->content.d = d;
}

static int calculate_int_int(int a, int b, char *op)
{
	if (strncmp(op, "+", 1) == 0)
	{
		return a + b;
	}
	else if (strncmp(op, "-", 1) == 0)
	{
		return a - b;
	}
	else if (strncmp(op, "**", 2) == 0)
	{
		return (int)pow((double)a, (double)b);
	}
	else if (strncmp(op, "*", 1) == 0)
	{
		return a * b;
	}
	else if (strncmp(op, "/", 1) == 0)
	{
		if (b != 0)
		{
			return a / b;
		}
		else
		{
			perror("Error: Division by zero\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("Error: Invalid operator\n");
		exit(EXIT_FAILURE);
	}
}

double calculate_int_double(int a, double b, char *op)
{
	return calculate_double_double((double)a, b, op);
}

double calculate_double_int(double a, int b, char *op)
{
	return calculate_double_double(a, (double)b, op);
}

double calculate_double_double(double a, double b, char *op)
{
	if (strncmp(op, "+", 1) == 0)
	{
		return a + b;
	}
	else if (strncmp(op, "-", 1) == 0)
	{
		return a - b;
	}
	else if (strncmp(op, "**", 2) == 0)
	{
		return pow(a, b);
	}
	else if (strncmp(op, "*", 1) == 0)
	{
		return a * b;
	}
	else if (strncmp(op, "/", 1) == 0)
	{
		if (b != 0)
		{
			return a / b;
		}
		else
		{
			perror("Error: Division by zero\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("Error: Invalid operator\n");
		exit(EXIT_FAILURE);
	}
}

static void free_ctrl_cell(CtrlCell *cell)
{
	if (cell->type == ENV)
	{
		free_hash_table(cell->content.env->rename_rules);
		cell->content.env->rename_rules = NULL;

		free(cell->content.env);
		cell->content.env = NULL;
	}
	else if (cell->type == LAMBDA)
	{
		for (size_t i = 0; i < cell->content.lambda->param_cnt; ++i)
		{
			free(cell->content.lambda->params[i]);
		}

		free(cell->content.lambda);
		cell->content.lambda = NULL;
	}
	else if (cell->type == DELTA)
	{
		CtrlCell *t1 = cell->content.delta->cell, *t2;
		if (t1 != NULL) {
			t1->prev->next = NULL;
			t1->prev = NULL;
		}
		
		while (t1 != NULL)
		{
			t2 = t1;
			t1 = t1->next;
			free_ctrl_cell(t2);
		}

		cell->content.delta = NULL;
	}
	else if (cell->type == TAU)
	{
		free(cell->content.tau);
		cell->content.tau = NULL;
	}

	free(cell);
	cell = NULL;
}
