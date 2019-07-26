require 'fiddle/import'

module M
  extend Fiddle::Importer
  dlload 'libsudare.so'
  extern 'int sudare_init_sdk(const char* dst)'
  extern 'int sudare_set_led_rect(int x, int y, int z, int rgb)'
  extern 'int sudare_set_led_polar(int a, int r, int h, int rgb)'
  extern 'int sudare_clear(void)'
  extern 'int sudare_send(void)'
  extern 'int sudare_sleep(int ms)'
end

exit(1) unless M.sudare_init_sdk('localhost:5510').zero?

loop do
  color = [0xFF0000, 0xFFFF00, 0x00FF00, 0x00FFFF, 0x0000FF, 0xFF00FF].sample
  60.times do |a|
    exit(1) unless M.sudare_clear.zero?
    100.times do |y|
      exit(1) unless M.sudare_set_led_polar(a, 14, y, color).zero?
    end
    exit(1) unless M.sudare_send.zero?
    exit(1) unless M.sudare_sleep(100).zero?
  end
end
