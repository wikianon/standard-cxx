#include <iostream>

uint32_t IPToUInt(const std::string ip) {
    int a, b, c, d;
    uint32_t addr = 0;

    if (sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
        return 0;

    addr = a << 24;
    addr |= b << 16;
    addr |= c << 8;
    addr |= d;
    return addr;
}

bool IsIPInRange(const std::string ip, const std::string network, const std::string mask)
{
    uint32_t ip_addr = IPToUInt(ip);
    uint32_t network_addr = IPToUInt(network);
    uint32_t mask_addr = IPToUInt(mask);

    uint32_t net_lower = (network_addr & mask_addr);
    uint32_t net_upper = (net_lower | (~mask_addr));

    if (ip_addr >= net_lower &&
        ip_addr <= net_upper)
        return true;
    return false;
}

void test(const std::string ip, const std::string network, const std::string mask, bool expected)
{
    if (IsIPInRange(ip, network, mask) != expected) {
        printf("Failed! %s %s %s %s\n", ip.c_str(), network.c_str(), mask.c_str(), expected ? "True" : "False");
    } else {
        printf("Success! %s %s %s %s\n", ip.c_str(), network.c_str(), mask.c_str(), expected ? "True" : "False");
    }
}

int main(int argc, char **argv)
{
    //std::string ip(argv[1]);

    test("192.168.15.3", "192.168.15.255", "255.255.255.0", true);
    test("192.168.15.3", "192.168.15.255", "255.255.255.0", false);

    return 0;
}
