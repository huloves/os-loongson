#ifndef _UAPI_BPF_H
#define _UAPI_BPF_H

#include <types.h>

enum bpf_cmd {
	BPF_PROG_LOAD,
};

enum bpf_prog_type {
	BPF_PROG_TYPE_UNSPEC,
	BPF_PROG_TYPE_KPROBE,
};

#define BPF_OBJ_NAME_LEN 16U

union bpf_attr {
	struct { /* anonymous struct used by BPF_MAP_CREATE command */
		uint32_t map_type;	/* one of enum bpf_map_type */
		uint32_t key_size;	/* size of key in bytes */
		uint32_t value_size;	/* size of value in bytes */
		uint32_t max_entries;	/* max number of entries in a map */
		uint32_t map_flags;	/* BPF_MAP_CREATE related
					 * flags defined above.
					 */
		uint32_t inner_map_fd;	/* fd pointing to the inner map */
		uint32_t numa_node;	/* numa node (effective only if
					 * BPF_F_NUMA_NODE is set).
					 */
		char map_name[BPF_OBJ_NAME_LEN];
		uint32_t map_ifindex;	/* ifindex of netdev to create on */
		uint32_t btf_fd;		/* fd pointing to a BTF type data */
		uint32_t btf_key_type_id;	/* BTF type_id of the key */
		uint32_t btf_value_type_id;	/* BTF type_id of the value */
		uint32_t btf_vmlinux_value_type_id;/* BTF type_id of a kernel-
						   * struct stored as the
						   * map value
						   */
		/* Any per-map-type extra fields
		 *
		 * BPF_MAP_TYPE_BLOOM_FILTER - the lowest 4 bits indicate the
		 * number of hash functions (if 0, the bloom filter will default
		 * to using 5 hash functions).
		 */
		uint64_t map_extra;
	};

	struct { /* anonymous struct used by BPF_PROG_LOAD command */
		uint32_t		prog_type;	/* one of enum bpf_prog_type */
		uint32_t		insn_cnt;
		uint64_t		insns;
		uint64_t		license;
		uint32_t		log_level;	/* verbosity level of verifier */
		uint32_t		log_size;	/* size of user buffer */
		uint64_t		log_buf;	/* user supplied buffer */
		uint32_t		kern_version;	/* not used */
		uint32_t		prog_flags;
		char			prog_name[BPF_OBJ_NAME_LEN];
		uint32_t		prog_ifindex;	/* ifindex of netdev to prep for */
		/* For some prog types expected attach type must be known at
		 * load time to verify attach type specific parts of prog
		 * (context accesses, allowed helpers, etc).
		 */
		uint32_t		expected_attach_type;
		uint32_t		prog_btf_fd;	/* fd pointing to BTF type data */
		uint32_t		func_info_rec_size;	/* userspace bpf_func_info size */
		uint64_t		func_info;	/* func info */
		uint32_t		func_info_cnt;	/* number of bpf_func_info records */
		uint32_t		line_info_rec_size;	/* userspace bpf_line_info size */
		uint64_t		line_info;	/* line info */
		uint32_t		line_info_cnt;	/* number of bpf_line_info records */
		uint32_t		attach_btf_id;	/* in-kernel BTF type id to attach to */
		union {
			/* valid prog_fd to attach to bpf prog */
			uint32_t	attach_prog_fd;
			/* or valid module BTF object fd or 0 to attach to vmlinux */
			uint32_t	attach_btf_obj_fd;
		};
		uint32_t		core_relo_cnt;	/* number of bpf_core_relo */
		uint64_t		fd_array;	/* array of FDs */
		uint64_t		core_relos;
		uint32_t		core_relo_rec_size; /* sizeof(struct bpf_core_relo) */
	};
};

#endif /* _UAPI_BPF_H */
