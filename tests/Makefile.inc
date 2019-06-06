
test: tester
	$(TESTER) <$(TESTS)/all_bytes.test
	$(TESTER) <$(TESTS)/basic.test
	$(TESTER) <$(TESTS)/one_missed.test
	$(TESTER) <$(TESTS)/one_missed_reversed.test
	$(TESTER) <$(TESTS)/other_unused_takeout.test
	$(TESTER) <$(TESTS)/unused_takeout.test
