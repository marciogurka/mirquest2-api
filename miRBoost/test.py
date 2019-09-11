import subprocess
import re

  	# ./gFoldmulti/gFold -s "../example.fa" -L 400 -DATA 1 > example_pos.txt
  	# ./gFoldmulti/gFold -s "../example.fa" -L 400 -DATA -1 > example_neg.txt
  	# cat example_pos.txt example_neg.txt > train_example.txt
	# head -100 example_pos.txt > test_example.txt
	# head -100 example_neg.txt >> test_example.txt
    # ./src/extractfeature.pl -s whole_selected_features.txt -i train_example.txt -o train_selected.txt  
  	# ./src/extractfeature.pl -s human_selected_features.txt -i train_example.txt -o train_selected.txt  
	# ./src/extractfeature.pl -s whole_selected_features.txt -i test_example.txt -o test_selected.txt
	# ./src/extractfeature.pl -s human_selected_features.txt -i test_example.txt -o test_selected.txt
    # ./miRBoost -i test_selected.txt -t train_selected.txt -d 0.25


command1 = "./gFoldmulti/gFold -s '../example.fa' -L 400 -DATA 1 > example_pos1.txt"
command2 = "./gFoldmulti/gFold -s '../example.fa' -L 400 -DATA -1 > example_neg1.txt"
command3 = "cat example_pos.txt example_neg1.txt > train_example1.txt"
command4 = "head -100 example_pos1.txt > test_example1.txt"
command5 = "head -100 example_neg1.txt >> test_example1.txt"
command6 = "./src/extractfeature.pl -s human_selected_features.txt -i train_example1.txt -o train_selected1.txt"
command7 = "./src/extractfeature.pl -s human_selected_features.txt -i test_example1.txt -o test_selected1.txt"
command8 = "./miRBoost -i test_selected1.txt -t train_selected1.txt -d 0.25"
fullComand = command1 + " ; " + command2 + " ; " + command3 + " ; " + command4 + " ; " + command5 + " ; " + command6 + " ; " + command7 + " ; " + command8 + " ; "
mirinhoResponse = subprocess.run([fullComand], shell=True, capture_output=True)
print(mirinhoResponse.returncode)
print("------------------\n")
pattern = "results.*.txt"
matches = re.findall(pattern, str(mirinhoResponse.stdout.decode('utf-8')))
print(str(mirinhoResponse.stdout.decode('utf-8')))
print("------------------\n")
print(matches[0])