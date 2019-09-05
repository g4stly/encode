#ifndef URL_ENCODER_H_
#define URL_ENCODER_H_

struct UrlEncoder {
	char *(*encode)(struct UrlEncoder *self, const char *input);
	char *(*decode)(struct UrlEncoder *self, const char *input);
};

extern const void *UrlEncoder;

#endif
