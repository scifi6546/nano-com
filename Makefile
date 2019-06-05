main:
	echo "nothing to do"
test:
	docker pull scifi6546/nano-com:test_container
	docker run scifi6546/nano-com:test_container
