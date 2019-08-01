PRODUCTS := cube_adapter contents_sdk proto_pi production_pi mini_pi

all:
	for PRODUCT in $(PRODUCTS); do\
		make -C "$(PWD)/$$PRODUCT";\
	done

clean:
	for PRODUCT in $(PRODUCTS); do\
		make -C "$(PWD)/$$PRODUCT" clean;\
	done
