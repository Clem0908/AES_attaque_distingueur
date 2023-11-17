OBJ=./objects
HD=./headers
SRC=./src
BIN=./bin
CFLAGS=-Wall -Wextra -O3
DIR=attaqueAES

chiffrer: clean compil
	@$(BIN)/AES chiffrer

chiffrer_CTR: clean compil
	@$(BIN)/AES chiffrer_CTR

dechiffrer: clean compil
	@$(BIN)/AES dechiffrer

attaquer_intersection: clean key compil
	@$(BIN)/AES attaquer 

attaquer_recursion: clean key compil
	@$(BIN)/AES attaquer recursion

key:
	@./generate_master_key.sh
	@echo 'Une nouvelle clé maître aléatoire a été générée'

compil: create_directory main.o CLI.o Substitution.o Attaque.o FileOperations.o AES.o Cadencement_cle.o Conversion.o XOR_operations.o Errors.o CTR.o 
	@gcc -pthread $(CFLAGS) -I $(HD) $(OBJ)/*.o -o $(BIN)/AES -lm
	@echo 'Compilation terminée'

create_directory:
	@mkdir -p $(BIN)/
	@mkdir -p $(OBJ)/
	@echo "Répertoires 'bin/' et 'objects/' créés"

main.o: $(SRC)/main.c
	@gcc -c $(CFLAGS) $< -o $(OBJ)/$@

%.o: $(SRC)/%.c $(HD)/%.h
	@gcc -c $(CFLAGS) $< -o $(OBJ)/$@

doxygen: 
	@doxygen Doxyfile
	@echo "Documentation générée. Pour la trouver, vous devez ouvrir 'html/index.html' via votre navigateur"

clean:
	@rm -rf $(OBJ)/
	@rm -rf $(BIN)/
	@rm -rf html/
	@rm -rf latex/
	@echo 'Nettoyage terminé'

zip:
	@mkdir $(DIR)
	@cp -r headers $(DIR)
	@cp -r src $(DIR)
	@cp -r in $(DIR)
	@cp -r rapport $(DIR)
	@cp README.md $(DIR)
	@cp Makefile $(DIR)
	@cp generate_master_key.sh $(DIR)
	@cp create_clear_cipher_couples.sh $(DIR)
	@cp Rapport.pdf $(DIR)
	@zip -r $(DIR) $(DIR)/*
	@rm -r $(DIR)
